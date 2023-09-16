
#include "Level/MazeGenerator.h"

#include "Actor/Event/EventTypes.h"
#include "Actor/Event/Event_DungeonExit.h"
#include "Actor/Ground/GroundBase.h"
#include "Actor/Event/EventBase.h"
#include "Actor/Item/ItemBase.h"

#include "Math/Math.h"

#include "Algorithm/algo.h"

#include "Framework/GameStateBase.h"
#include "Framework/World.h"

#include "GameSettings.h"

#include "Component/SpriteComponent.h"

using namespace DirectX;
using namespace Math;
using namespace Algo;

#define _DEBUG 1

#if _DEBUG
#include <format>
#endif

#define JOINED_BLOCK(x) x == EBlockJoinState::Joined

MazeGenerator::MazeGenerator(DirectX11& dx)
	: Level2D(dx)
{
}
MazeGenerator::~MazeGenerator()
{
	pDX = nullptr;

	pExit.reset();
	pExit = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void MazeGenerator::BeginPlay(DirectX11& dx)
{
	StartMoveToNextFloor();
}

void MazeGenerator::GenerateGroundLayer()
{
	Clear();

	InitializeMaze();
	InitializeBlock();

#if _DEBUG
	ShowBlock();

	ShowTiles();
#endif
}
void MazeGenerator::GenerateEventLayer()
{
	SetEnterExit();

	SetEnterBlockEvent();
}
void MazeGenerator::GenerateItemLayer()
{
}

FOnDungeonNextFloor& MazeGenerator::GetDungeonNextFloor()
{
	return OnDungeonNextFloor;
}
FOnEnterBlock& MazeGenerator::GetEnterBlock()
{
	return OnEnterBlock;
}
void MazeGenerator::EnterBlock(const FEventData& inEventData)
{
	OnEnterBlock.Broadcast(FVector2D(centerX, centerY));
}

// ------------------------------------------------------
// Main : Ground Layer
// ------------------------------------------------------

// --------------------------
// Main : Ground Layer : Initialization
// --------------------------
void MazeGenerator::Clear()
{
	Level2D::Clear();

	actualBlockCountX = 0;
	actualBlockCountY = 0;

	actualBlockCount = 0;

	actualMazeCountX = 0;
	actualMazeCountY = 0;

	actualMazeCount = 0;

	blockIDs.Clear();
	blockUnionFind.clear();

	RoomLocalRects.Clear();
	actualRoomCount = 0;

	pExit.reset();
	pExit = nullptr;
}
void MazeGenerator::InitializeMaze()
{
	actualBlockCountX = (uint16_t)RandRange(GameSettings::BLOCK_MIN_COUNT, GameSettings::BLOCK_MAX_COUNT_X);
	actualBlockCountY = (uint16_t)RandRange(GameSettings::BLOCK_MIN_COUNT, GameSettings::BLOCK_MAX_COUNT_Y);
	actualBlockCount = actualBlockCountX * actualBlockCountY;

	actualMazeCountX = actualBlockCountX * GameSettings::BLOCK_SIZE;
	actualMazeCountY = actualBlockCountY * GameSettings::BLOCK_SIZE;
	actualMazeCount = actualMazeCountX * actualMazeCountY;

	TArray<FBlock> Rows(actualBlockCountX, FBlock(EBlockID::Path));

	blockIDs.Clear();
	blockIDs.Init(actualBlockCountY, Rows);

	blockUnionFind = UnionFind(actualBlockCount);

	TArray<EGroundTile> mazeRows(actualMazeCountX, EGroundTile::None);
	mazeTils.Init(actualMazeCountY, mazeRows);

#if _DEBUG
	OutputDebugStringA(std::format("block size : x:{} y:{} s:{}\n", actualBlockCountX, actualBlockCountY, actualBlockCount).c_str());
	OutputDebugStringA(std::format("maze size: x:{} y:{} s:{}\n", actualMazeCountX, actualMazeCountY, actualMazeCount).c_str());
#endif

	Init(actualMazeCountX, actualMazeCountY);
}

// --------------------------
// Main : Ground Layer : Block Initialization
// --------------------------
void MazeGenerator::InitializeBlock()
{
	const int blockRoomPlanNum = RandRange(actualBlockCount / 4, actualBlockCount / 2);
	int x = 0, y = 0;
	for (int i = 0; i < blockRoomPlanNum; ++i)
	{
		x = Random(actualBlockCountX - 1);
		y = Random(actualBlockCountY - 1);
		if (blockIDs[y][x].id == EBlockID::Path)
		{
			blockIDs[y][x].id = EBlockID::Room;
			++actualRoomCount;
		}
	};

	x = actualBlockCountX;
	y = actualBlockCountY;
	if (x > y)
	{
		Algo::swap(x, y);
	}
	if (x > 1)
	{
		x /= 2;
	}
	const int unusedBlockNum = RandRange(1, x);
	for (int i = 0; i < unusedBlockNum; ++i)
	{
		x = Random(actualBlockCountX - 1);
		y = Random(actualBlockCountY - 1);
		if (blockIDs[y][x].id == EBlockID::Path)
		{
			blockIDs[y][x].id = EBlockID::None;
		}
	}

	JoinBlock();
	JoinBlockAdditional();
	JoinClosedBlock();

	MakeRoom();

	MakePath();

	MakeWall();
}

void MazeGenerator::JoinBlock()
{
	const uint16_t sx = (uint16_t)Random(actualBlockCountX - 1);
	const uint16_t sy = (uint16_t)Random(actualBlockCountY - 1);

	if (blockIDs[sy][sx].id == EBlockID::None)
	{
		JoinBlock();
		return;
	}

	TArray<EDirection> JoinDirections;
	GetJoinableDirection(JoinDirections, sx, sy);
	JoinBlock(sx, sy, JoinDirections.RandomValue());

	for (int y = 0; y < actualBlockCountY; ++y)
	{
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			if (blockIDs[y][x].id != EBlockID::None)
			{
				if (!blockIDs[y][x].IsJoinedAtLeastTwo())
				{
					JoinBlock();
					return;
				}
			}
		}
	}
}
bool MazeGenerator::JoinBlock(const uint16_t& x, const uint16_t& y, const EDirection& direction)
{
	switch (direction)
	{
	case EDirection::Left:
		blockIDs[y][x].JoinL = EBlockJoinState::Joined;
		blockIDs[y][x - 1].JoinR = EBlockJoinState::Joined;
		blockUnionFind.unite(GetPosBlock(x, y), GetPosBlock(x - 1, y));
		return true;
	case EDirection::Right:
		blockIDs[y][x].JoinR = EBlockJoinState::Joined;
		blockIDs[y][x + 1].JoinL = EBlockJoinState::Joined;
		blockUnionFind.unite(GetPosBlock(x, y), GetPosBlock(x + 1, y));
		return true;
	case EDirection::Up:
		blockIDs[y][x].JoinU = EBlockJoinState::Joined;
		blockIDs[y - 1][x].JoinD = EBlockJoinState::Joined;
		blockUnionFind.unite(GetPosBlock(x, y), GetPosBlock(x, y - 1));
		return true;
	case EDirection::Down:
		blockIDs[y][x].JoinD = EBlockJoinState::Joined;
		blockIDs[y + 1][x].JoinU = EBlockJoinState::Joined;
		blockUnionFind.unite(GetPosBlock(x, y), GetPosBlock(x, y + 1));
		return true;
	default:
		break;
	}
	return false;
}
void MazeGenerator::GetJoinableDirection(TArray<EDirection>& out, const uint16_t& sx, const uint16_t& sy)
{
	auto AddToDirection = [this, &out, sx, sy](const int& x, const int& y, const EDirection& Dir)
	{
		bool isAdded = false;
		if (IsInBlock((uint16_t)sx + x, (uint16_t)sy + y))
		{
			if (blockIDs[(uint16_t)sy + y][(uint16_t)sx + x].id != EBlockID::None)
			{
				out.Add(Dir);
				isAdded = true;
			}
		}
		if (!isAdded)
		{
			switch (Dir)
			{
			case EDirection::Left:
				blockIDs[sy][sx].JoinL = EBlockJoinState::OutOfRange;
				break;
			case EDirection::Right:
				blockIDs[sy][sx].JoinR = EBlockJoinState::OutOfRange;
				break;
			case EDirection::Up:
				blockIDs[sy][sx].JoinU = EBlockJoinState::OutOfRange;
				break;
			case EDirection::Down:
				blockIDs[sy][sx].JoinD = EBlockJoinState::OutOfRange;
				break;
			default:
				break;
			}
		}
	};

	AddToDirection(-1, 0, EDirection::Left);
	AddToDirection(1, 0, EDirection::Right);
	AddToDirection(0, -1, EDirection::Up);
	AddToDirection(0, 1, EDirection::Down);
}
void MazeGenerator::JoinBlockAdditional()
{
	TArray<uint16_t> candidateJoinBlock;
	for (int y = 0; y < actualBlockCountY; ++y)
	{
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			const FBlock& t = blockIDs[y][x];
			if (!t.IsJoinedAll() && t.id != EBlockID::None)
			{
				candidateJoinBlock.Add(GetPosBlock(x, y));
			}
		}
	}

	if (candidateJoinBlock.Size() >= 2)
	{
		const uint16_t joinPlanAdditional = (uint16_t)RandRange(1, candidateJoinBlock.Size() / 2);

		uint16_t x = 0, y = 0;
		TArray<EDirection> JoinableDirection;
		for (int i = 0; i < joinPlanAdditional; ++i)
		{
			const uint16_t candidateIdx = (uint16_t)Random(candidateJoinBlock.Size() - 1);
			const uint16_t pos = candidateJoinBlock[candidateIdx];
			GetBlockXY(pos, x, y);

			GetJoinableDirection(JoinableDirection, x, y);
			if (JoinableDirection.Size() > 0)
			{
				JoinBlock(x, y, JoinableDirection.RandomValue());
			}

			candidateJoinBlock.Remove(candidateIdx);
			JoinableDirection.Clear();
		}
	}
}
bool MazeGenerator::JoinClosedBlock()
{
	if (blockUnionFind.numOfParent() == 1)
	{
		return false;
	}
	else
	{
		std::vector<int> parents;
		blockUnionFind.GetParent(parents);

		uint16_t x = 0, y = 0;
		TArray<EDirection> direction;
		for (int y = 0; y < actualBlockCountY; ++y)
		{
			for (int x = 0; x < actualBlockCountX; ++x)
			{
				if (blockIDs[y][x].id != EBlockID::None)
				{
					GetJoinableDirection(direction, x, y);
					if (direction.Size() > 0)
					{
						const EDirection dir = direction.RandomValue();
						bool IsSame = true;
						switch (dir)
						{
						case EDirection::Left:
							IsSame = blockUnionFind.IsSame(GetPosBlock(x, y), GetPosBlock(x - 1, y));
							break;
						case EDirection::Right:
							IsSame = blockUnionFind.IsSame(GetPosBlock(x, y), GetPosBlock(x + 1, y));
							break;
						case EDirection::Up:
							IsSame = blockUnionFind.IsSame(GetPosBlock(x, y), GetPosBlock(x, y - 1));
							break;
						case EDirection::Down:
							IsSame = blockUnionFind.IsSame(GetPosBlock(x, y), GetPosBlock(x, y + 1));
							break;
						default:
							break;
						}
						if (!IsSame)
						{
							JoinBlock(x, y, dir);
							if (!JoinClosedBlock())
							{
								return true;
							}
						}
					}
					direction.Clear();
				}
			}
		}
	}
	return false;
}

// --------------------------
// Main : Ground Layer : Room
// --------------------------
void MazeGenerator::MakeRoom()
{
	const int blockLeftTop = 2;
	const int blockRightBottom = GameSettings::BLOCK_SIZE - 2;

	for (int y = 0; y < actualBlockCountY; ++y)
	{
		TArray<FRect> BlockRect;
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			const FBlock& block = blockIDs[y][x];
			if (block.id == EBlockID::Room)
			{
				const int roomPlanLeftTopX = RandRange(blockLeftTop, blockRightBottom / 2);
				const int roomPlanLeftTopY = RandRange(blockLeftTop, blockRightBottom / 2);

				const int actualRoomWidth = blockRightBottom - roomPlanLeftTopX;
				const int actualRoomHeight = blockRightBottom - roomPlanLeftTopY;

				const int actualRoomLeftTopX = RandRange(blockLeftTop, roomPlanLeftTopX);
				const int actualRoomLeftTopY = RandRange(blockLeftTop, roomPlanLeftTopY);

				const int actualRoomRightBottomX = actualRoomLeftTopX + actualRoomWidth;
				const int actualRoomRightBottomY = actualRoomLeftTopY + actualRoomHeight;

				BlockRect.Add(FRect(actualRoomLeftTopX, actualRoomLeftTopY, actualRoomRightBottomX, actualRoomRightBottomY));
#if _DEBUG
				//OutputDebugStringA(std::format("RoomRect : {} {} {} {}\n", actualRoomLeftTopX, actualRoomLeftTopY, actualRoomRightBottomX, actualRoomRightBottomY).c_str());
#endif

				const uint16_t Sy = BlockToMaze(y, actualRoomLeftTopY);
				const uint16_t Ey = BlockToMaze(y, actualRoomRightBottomY);

				const uint16_t Sx = BlockToMaze(x, actualRoomLeftTopX);
				const uint16_t Ex = BlockToMaze(x, actualRoomRightBottomX);

				for (int constantY = Sy; constantY <= Ey; ++constantY)
				{
					SetGroundLayerID(ConvertToGround(EGroundTile::Room, mGroundType), Sx, Ex, constantY, true);
				}
			}
			else if (block.id == EBlockID::Path)
			{
				const int rectX = RandRange(blockLeftTop, blockRightBottom);
				const int rectY = RandRange(blockLeftTop, blockRightBottom);
				BlockRect.Add(FRect(rectX, rectY, rectX, rectY));

				int px = BlockToMaze(x, rectX);
				int py = BlockToMaze(y, rectY);
				SetGroundLayerIDSpe(ConvertToGround(EGroundTile::Path, mGroundType), px, py);
			}
			else
			{
				BlockRect.Add(FRect(x, y, x, y));
#if _DEBUG
				//OutputDebugStringA(std::format("None {} {}\n", x, y).c_str());
#endif
			}
		}
		RoomLocalRects.Add(BlockRect);
	}
}

// --------------------------
// Main : Ground Layer : Path
// --------------------------
void MazeGenerator::MakePath()
{
	for (int y = 0; y < actualBlockCountY; ++y)
	{
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			const FBlock& block = blockIDs[y][x];
			if (block.id == EBlockID::Room || block.id == EBlockID::Path)
			{
				if (JOINED_BLOCK(block.JoinR) || JOINED_BLOCK(block.JoinD))
				{
					MakePath(x, y, block);
				}
			}
		}
	}
}
void MazeGenerator::MakePath(const uint16_t& x, const uint16_t& y, const FBlock& block)
{
	if (!IsInBlock(x, y))
	{
		return;
	}
	const FRect& roomRect = RoomLocalRects[y][x];

	/* Draw Right Direction Path */
	uint16_t currSx, currSy, nextSx, nextSy;
	if (JOINED_BLOCK(block.JoinR))
	{
		if (IsInBlock(x + 1, y))
		{
			GetPathStartPos(roomRect, EDirection::Right, currSx, currSy);

			const FBlock& nextBlock = blockIDs[y][x + 1];
			const FRect& nextRoomRect = RoomLocalRects[y][x + 1];
			GetPathStartPos(nextRoomRect, EDirection::Left, nextSx, nextSy);

			currSx = BlockToMaze(x, currSx);
			currSy = BlockToMaze(y, currSy);

			nextSx = BlockToMaze(x + 1, nextSx);
			nextSy = BlockToMaze(y, nextSy);

			int borderX = RandRange(currSx + 1, nextSx - 1);

			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), currSx, borderX, currSy, true);
			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), borderX, nextSx, nextSy, true);

			// Merge Path
			if (currSy > nextSy)
			{
				swap(currSy, nextSy);
			}
			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), currSy, nextSy, borderX);
		}
	}
	/* Draw Down Direction Path */
	if (JOINED_BLOCK(block.JoinD))
	{
		if (IsInBlock(x, y + 1))
		{
			GetPathStartPos(roomRect, EDirection::Down, currSx, currSy);

			const FBlock& nextBlock = blockIDs[y + 1][x];
			const FRect& nextRoomRect = RoomLocalRects[y + 1][x];
			GetPathStartPos(nextRoomRect, EDirection::Up, nextSx, nextSy);

			currSx = BlockToMaze(x, currSx);
			currSy = BlockToMaze(y, currSy);

			nextSx = BlockToMaze(x, nextSx);
			nextSy = BlockToMaze(y + 1, nextSy);

			int borderY = RandRange(currSy + 1, nextSy - 1);

			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), currSy, borderY, currSx);
			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), borderY, nextSy, nextSx);

			// Merge Path
			if (currSx > nextSx)
			{
				swap(currSx, nextSx);
			}
			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), currSx, nextSx, borderY, true);
		}
	}
}
void MazeGenerator::GetPathStartPos(const FRect& inRect, const EDirection& direction, uint16_t& x, uint16_t& y)
{
	TArray<int> candidatePathStartPos;
	switch (direction)
	{
	case EDirection::Right:
		for (int roomY = inRect.top; roomY <= inRect.bottom; ++roomY)
		{
			candidatePathStartPos.Add(GetPosMaze(inRect.right + 1, roomY));
		}
		break;
	case EDirection::Left:
		for (int roomY = inRect.top; roomY <= inRect.bottom; ++roomY)
		{
			candidatePathStartPos.Add(GetPosMaze(inRect.left - 1, roomY));
		}
		break;
	case EDirection::Up:
		for (int roomX = inRect.left; roomX <= inRect.right; ++roomX)
		{
			candidatePathStartPos.Add(GetPosMaze(roomX, inRect.top - 1));
		}
		break;
	case EDirection::Down:
		for (int roomX = inRect.left; roomX <= inRect.right; ++roomX)
		{
			candidatePathStartPos.Add(GetPosMaze(roomX, inRect.bottom + 1));
		}
		break;
	default:
		break;
	}

	GetMazeXY(candidatePathStartPos.RandomValue(), x, y);
}

// --------------------------
// Main : Ground Layer : Wall
// --------------------------
void MazeGenerator::MakeWall()
{
	const auto layer = pObjectManager->pObjects[Layer::EActorLayer::Background];
	for (const auto& actor : layer)
	{
		if (actor != nullptr)
		{
			if (const auto& actor2d = static_pointer_cast<Actor2D>(actor))
			{
				auto p = actor2d->Get2DIdx();
				int x = p.x, y = p.y;
				if (const auto& ground = static_pointer_cast<GroundBase>(actor2d))
				{
					const auto& tile = ConvertToGroundTile(ground->GetGroundType());
					switch (tile)
					{
					case EGroundTile::Room:
					case EGroundTile::Path:
						MakeWall(x, y, -1,  0);
						MakeWall(x, y,  1,  0);
						MakeWall(x, y,  0, -1);
						MakeWall(x, y,  0,  1);
						MakeWall(x, y, -1, -1);
						MakeWall(x, y,  1, -1);
						MakeWall(x, y, -1,  1);
						MakeWall(x, y,  1,  1);
						break;
					default:
						break;
					}
				}
			}
		}
	}
}
void MazeGenerator::MakeWall(const int& x, const int& y, const int& addX, const int& addY)
{
	const auto my = y + addY;
	const auto mx = x + addX;
	if (mazeTils[my][mx] == EGroundTile::None)
	{
		auto actor = SetGroundLayerIDSpe(ConvertToGround(EGroundTile::Wall, mGroundType), mx, my);
		const auto normal = ConvertToDirection(addX, addY);
		actor->SetNormal(normal);
		switch (normal)
		{
		case EDirection::Left:
			break;
		case EDirection::Right:
			actor->SetActorRotation({ 180.f, 0.f, 0.f });
			break;
		case EDirection::Up:
			actor->SetActorRotation({ 90.f, 0.f, 0.f });
			break;
		case EDirection::Down:
			actor->SetActorRotation({ -90.f, 0.f, 0.f });
			break;
		case EDirection::LeftUp:
			actor->SetActorRotation({ 135.f, 0.f, 0.f });
			break;
		case EDirection::LeftDown:
			break;
		case EDirection::RightUp:
			break;
		case EDirection::RightDown:
			break;
		default:
			break;
		}
	}
}

// ------------------------------------------------------
// Main : Event Layer
// ------------------------------------------------------
void MazeGenerator::SetEnterBlockEvent()
{
	for (int blockY = 0; blockY < actualBlockCountY; ++blockY)
	{
		for (int blockX = 0; blockX < actualBlockCountX; ++blockX)
		{
			const auto& block = blockIDs[blockY][blockX];
			switch (block.id)
			{
			case EBlockID::Path:
				CheckSetEnterBlockCount(blockX, blockY);
				break;
			case EBlockID::Room:
				CheckSetEnterBlockCount(blockX, blockY);
				break;
			default:
				break;
			}
		}
	}
}
void MazeGenerator::SetEnterBlockEvent(const int& worldX, const int& worldY)
{
	auto EventData = GetWorld()->SpawnActor<EventBase>(*pDX, EEventId::EnterRoom);
	EventData->BeginPlay(*pDX);
	EventData->GetOnEnterVolume().Bind<&MazeGenerator::EnterBlock>(*this);
	SetEventLayerID(EventData, worldX, worldY);
}
void MazeGenerator::CheckSetEnterBlockCount(const int& blockX, const int& blockY)
{
	const auto& rect = RoomLocalRects[blockY][blockX];
	const auto& block = blockIDs[blockY][blockX];
	for (int ry = rect.top; ry <= rect.bottom; ++ry)
	{
		for (int rx = rect.left; rx <= rect.right; ++rx)
		{
			int x = BlockToMaze(blockX, rx);
			int y = BlockToMaze(blockY, ry);
			if (CheckIsEnter(block.id, x, y))
			{
				SetEnterBlockEvent(x, y);
			}
		}
	}
}

void MazeGenerator::SetEnterExit()
{
	bool makeEnter = false, makeExit = false;

	float additionalPossibility = 1.f / actualRoomCount;
	float enterPossibility = 1.f / actualRoomCount;

	float exitPossibility = enterPossibility - 0.25f;

	FRect localRect;
	int blockX = 0, blockY = 0;
	bool ima = false;
	for (int y = 0; y < actualBlockCountY; ++y)
	{
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			const FBlock& block = blockIDs[y][x];
			if (block.id == EBlockID::Room)
			{
				localRect = RoomLocalRects[y][x];
				blockX = x;
				blockY = y;
				if (!makeEnter)
				{
					if (RandomBool(enterPossibility))
					{
						SetEnter(blockX, blockY);

						makeEnter = true;
						ima = true;
					}
				}
				if (!makeEnter)
				{
					enterPossibility += additionalPossibility;
				}
				if (!makeExit)
				{
					if (!ima)
					{
						if (RandomBool(exitPossibility))
						{
							SetExit(x, y);
							makeExit = true;
						}
					}
				}
				if (!makeExit)
				{
					exitPossibility += additionalPossibility;
				}
				ima = false;
			}
			if (makeEnter && makeExit)
			{
				return;
			}
		}
	}
	if (!makeEnter)
	{
		SetEnter(blockX, blockY);
	}
	if (!makeExit)
	{
		SetExit(blockX, blockY);
	}
}
void MazeGenerator::SetEnter(const uint16_t& blockX, const uint16_t& blockY)
{
	const FRect& localRect = RoomLocalRects[blockY][blockX];
	uint16_t resX = BlockToMaze(blockX, RandRange(localRect.left, localRect.right));
	uint16_t resY = BlockToMaze(blockY, RandRange(localRect.top, localRect.bottom));
	if (CheckIsEnter(blockIDs[blockY][blockX].id, resX, resY))
	{
		SetEnter(blockX, blockY);
	}
	else
	{
		SetEventLayerID(EEventId::Enter, resX, resY);
		SetStartPosition(FVector(resX, resY, 0));
	}
}
void MazeGenerator::SetExit(const uint16_t& blockX, const uint16_t& blockY)
{
	const FRect& localRect = RoomLocalRects[blockY][blockX];
	uint16_t resX = BlockToMaze(blockX, RandRange(localRect.left, localRect.right));
	uint16_t resY = BlockToMaze(blockY, RandRange(localRect.top, localRect.bottom));

	pExit = GetWorld()->SpawnActor<Event_DungeonExit>(*pDX);
	pExit->BeginPlay(*pDX);
	pExit->SetOuter(this);
	SetEventLayerID(pExit, resX, resY);

	pExit->OnChoiceYes.Bind<&MazeGenerator::StartMoveToNextFloor>(*this, "MazeExit");
}

void MazeGenerator::StartMoveToNextFloor()
{
	if (pExit)
	{
		pExit->OnChoiceYes.Unbind("MazeExit");
	}

	Generate(*pDX);

	//GetWorld()->GetGameState()->OnLandmarkClosed.Bind<&MazeGenerator::CompletedMoveToNextFloor>(*this, "MazeGenerator");
	//GetWorld()->GetGameState()->OpenLandmarkUI(*pDX, L"", 0.5f);
}

void MazeGenerator::CompletedMoveToNextFloor()
{
	Generate(*pDX);

	OnDungeonNextFloor.Broadcast(this);
}

// ------------------------------------------------------
// Main : Item Layer
// ------------------------------------------------------
void MazeGenerator::SpawnItems()
{
	uint16_t resX, resY;
	FRect localRect;
	int blockX = 0, blockY = 0;
	for (int y = 0; y < actualBlockCountY; ++y)
	{
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			const FBlock& block = blockIDs[y][x];
			if (block.id == EBlockID::Room)
			{
				localRect = RoomLocalRects[y][x];
				blockX = x;
				blockY = y;
				resX = BlockToMaze(x, RandRange(localRect.left, localRect.right));
				resY = BlockToMaze(y, RandRange(localRect.top, localRect.bottom));

				SetEventLayerID(EEventId::Enter, resX, resY);
			}
		}
	}
}

// --------------------------
// Main : Debug
// --------------------------
void MazeGenerator::ShowBlock()
{
#if _DEBUG
	OutputDebugStringA("-------- Show Block -------\n");
	for (int y = 0; y < actualBlockCountY; ++y)
	{
		std::string str, strB;
		for (int x = 0; x < actualBlockCountX; ++x)
		{
			const FBlock& t = blockIDs[y][x];

			if (x > 1 && x < actualBlockCountX)
			{
				if (JOINED_BLOCK(t.JoinL))
				{
					str += "+";
				}
				else
				{
					str += " ";
				}
			}

			switch (t.id)
			{
			case EBlockID::None:
				str += " ";
				break;
			case EBlockID::Room:
				str += "R";
				break;
			case EBlockID::Path:
				str += "C";
				break;
			default:
				break;
			}
			if (x == 0)
			{
				if (JOINED_BLOCK(t.JoinR))
				{
					str += "+";
				}
				else
				{
					str += " ";
				}
			}
			if (JOINED_BLOCK(t.JoinD))
			{
				strB += "+ ";
			}
			else
			{
				strB += "  ";
			}
		}
		OutputDebugStringA((str + "\n").c_str());
		OutputDebugStringA((strB + "\n").c_str());
	}
	OutputDebugStringA("-------- Show Block -------\n");
	OutputDebugStringA("\n\n");
#endif
}

// --------------------------
// Main : Ground Layer : Utils
// --------------------------
std::shared_ptr<GroundBase> MazeGenerator::SetGroundLayerIDSpe(const EGroundId& groundType, const float& worldX, const float& worldY)
{
	mazeTils[worldY][worldX] = ConvertToGroundTile(groundType);
	return Level2D::SetGroundLayerIDSpe(groundType, worldX, worldY);
}
bool MazeGenerator::IsInMaze(const uint16_t& x, const uint16_t& y) const noexcept
{
	return (x >= 0 && x < actualMazeCountX) && (y >= 0 && y < actualMazeCountY);
}
bool MazeGenerator::IsInBlock(const uint16_t& x, const uint16_t& y) const noexcept
{
	return (x >= 0 && x < actualBlockCountX) && (y >= 0 && y < actualBlockCountY);
}

bool MazeGenerator::CheckIsEnter(const EBlockID& blockID, const int& worldX, const int& worldY) const noexcept
{
	if (IsInWorld(worldX, worldY))
	{
		//const auto& groundData = GroundLayer[worldY][worldX];
		//if (groundData != nullptr)
		//{
		//	int PathCount = 0, RoomCount = 0;

		//	auto CheckIsEnter = [this, &PathCount, &RoomCount](const int& worldX, const int& worldY)
		//	{
		//		auto groundData = GroundLayer[worldY][worldX];
		//		if (groundData != nullptr)
		//		{
		//			switch (ConvertToGroundTile(groundData->GetGroundType()))
		//			{
		//			case EGroundTile::Path:
		//				++PathCount;
		//				break;
		//			case EGroundTile::Room:
		//				++RoomCount;
		//				break;
		//			default:
		//				break;
		//			}
		//		}
		//	};
		//	CheckIsEnter(worldX - 1, worldY);
		//	CheckIsEnter(worldX + 1, worldY);
		//	CheckIsEnter(worldX, worldY - 1);
		//	CheckIsEnter(worldX, worldY + 1);

		//	switch (blockID)
		//	{
		//	case EBlockID::Path:
		//		if (PathCount >= 3)
		//		{
		//			return true;
		//		}
		//		break;
		//	case EBlockID::Room:
		//		if (RoomCount >= 2 && PathCount >= 1)
		//		{
		//			return true;
		//		}
		//		break;
		//	default:
		//		break;
		//	}
		//}
	}
	return false;
}

void MazeGenerator::GetMazeXY(const uint16_t& pos, uint16_t& x, uint16_t& y) const noexcept
{
	// 28 % 5 = 3
	x = (uint16_t)(pos % actualMazeCountX);
	// 28 / 5 = 5
	y = (uint16_t)(pos / actualMazeCountX);
}
void MazeGenerator::GetBlockXY(const uint16_t& pos, uint16_t& x, uint16_t& y) const noexcept
{
	x = (uint16_t)(pos % actualBlockCountX);
	y = (uint16_t)(pos / actualBlockCountX);
}

uint16_t MazeGenerator::GetPosMaze(const uint16_t& x, const uint16_t& y) const noexcept
{
	return y * actualMazeCountX + x;
}
uint16_t MazeGenerator::GetPosBlock(const uint16_t& x, const uint16_t& y) const noexcept
{
	return y * actualBlockCountX + x;
}

uint16_t MazeGenerator::BlockToMaze(const uint16_t& blockXorY, const uint16_t& offsetXorY) const noexcept
{
	return blockXorY * GameSettings::BLOCK_SIZE + offsetXorY;
}
uint16_t MazeGenerator::MazeToBlock(const uint16_t& worldXorY, const uint16_t& blockXorY) const noexcept
{
	return worldXorY / GameSettings::BLOCK_SIZE - blockXorY;
}

// ------------------------------------------------------
// Sub Structure
// ------------------------------------------------------

// --------------------------
// Sub Structure : Block
// --------------------------
MazeGenerator::FBlock::FBlock(EBlockID inID)
	: id(inID), JoinL(EBlockJoinState::None), JoinR(EBlockJoinState::None), JoinU(EBlockJoinState::None), JoinD(EBlockJoinState::None)
{
}
bool MazeGenerator::FBlock::IsJoinedAll() const noexcept
{
	return JoinL != EBlockJoinState::None && JoinR != EBlockJoinState::None && JoinU != EBlockJoinState::None && JoinD != EBlockJoinState::None;
}
bool MazeGenerator::FBlock::IsJoinedAtLeastTwo() const noexcept
{
	int x = 0;
	x += JoinL != EBlockJoinState::None;
	x += JoinR != EBlockJoinState::None;
	x += JoinU != EBlockJoinState::None;
	x += JoinD != EBlockJoinState::None;
	return x > 1;
}
bool MazeGenerator::FBlock::IsJoinedOne() const noexcept
{
	int x = 0;
	x += JOINED_BLOCK(JoinL);
	x += JOINED_BLOCK(JoinR);
	x += JOINED_BLOCK(JoinU);
	x += JOINED_BLOCK(JoinD);
	return x == 1;
}
void MazeGenerator::FBlock::GetJoinableDirection(TArray<EDirection>& Out) const noexcept
{
	if (JoinL == EBlockJoinState::None)
	{
		Out.Add(EDirection::Left);
	}
	if (JoinR == EBlockJoinState::None)
	{
		Out.Add(EDirection::Right);
	}
	if (JoinU == EBlockJoinState::None)
	{
		Out.Add(EDirection::Up);
	}
	if (JoinD == EBlockJoinState::None)
	{
		Out.Add(EDirection::Down);
	}
}
