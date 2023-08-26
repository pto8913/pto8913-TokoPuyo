
#include "Level/MazeGenerator.h"

#include "Object/Sprite.h"
#include "Object/Event/EventTypes.h"
#include "Object/Event/Event_DungeonExit.h"

#include "Algorithm/algo.h"
#include "Math/Math.h"

#include "GameSettings.h"
#include "GameState/GameStateBase.h"
#include "Engine/World.h"

#include "Object/Ground/GroundBase.h"
#include "Object/Event/EventBase.h"
#include "Object/Item/ItemBase.h"

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
	actualBlockCountX = (UINT8)RandRange(GameSettings::BLOCK_MIN_COUNT, GameSettings::BLOCK_MAX_COUNT_X);
	actualBlockCountY = (UINT8)RandRange(GameSettings::BLOCK_MIN_COUNT, GameSettings::BLOCK_MAX_COUNT_Y);
	actualBlockCount = actualBlockCountX * actualBlockCountY;

	actualMazeCountX = actualBlockCountX * GameSettings::BLOCK_SIZE;
	actualMazeCountY = actualBlockCountY * GameSettings::BLOCK_SIZE;
	actualMazeCount = actualMazeCountX * actualMazeCountY;

	TArray<FBlock> Rows(actualBlockCountX, FBlock(EBlockID::Path));

	blockIDs.Clear();
	blockIDs.Init(actualBlockCountY, Rows);

	blockUnionFind = UnionFind(actualBlockCount);

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
}

void MazeGenerator::JoinBlock()
{
	const UINT8 sx = (UINT8)Random(actualBlockCountX - 1);
	const UINT8 sy = (UINT8)Random(actualBlockCountY - 1);

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
bool MazeGenerator::JoinBlock(const UINT8& x, const UINT8& y, const EDirection& direction)
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
void MazeGenerator::GetJoinableDirection(TArray<EDirection>& out, const UINT8& sx, const UINT8& sy)
{
	auto AddToDirection = [this, &out, sx, sy](const int& x, const int& y, const EDirection& Dir)
	{
		bool isAdded = false;
		if (IsInBlock((UINT8)sx + x, (UINT8)sy + y))
		{
			if (blockIDs[(UINT8)sy + y][(UINT8)sx + x].id != EBlockID::None)
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
	TArray<UINT16> candidateJoinBlock;
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
		const UINT8 joinPlanAdditional = (UINT8)RandRange(1, candidateJoinBlock.Size() / 2);

		UINT8 x = 0, y = 0;
		TArray<EDirection> JoinableDirection;
		for (int i = 0; i < joinPlanAdditional; ++i)
		{
			const UINT16 candidateIdx = (UINT16)Random(candidateJoinBlock.Size() - 1);
			const UINT16 pos = candidateJoinBlock[candidateIdx];
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

		UINT8 x = 0, y = 0;
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
				OutputDebugStringA(std::format("RoomRect : {} {} {} {}\n", actualRoomLeftTopX, actualRoomLeftTopY, actualRoomRightBottomX, actualRoomRightBottomY).c_str());
#endif

				const UINT16 Sy = BlockToMaze(y, actualRoomLeftTopY);
				const UINT16 Ey = BlockToMaze(y, actualRoomRightBottomY);

				const UINT16 Sx = BlockToMaze(x, actualRoomLeftTopX);
				const UINT16 Ex = BlockToMaze(x, actualRoomRightBottomX);

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
#if _DEBUG
				OutputDebugStringA(std::format("PathRect : {} {}\n", rectX, rectY).c_str());
#endif
				SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), BlockToMaze(x, rectX), BlockToMaze(y, rectY));
			}
			else
			{
				BlockRect.Add(FRect(x, y, x, y));
#if _DEBUG
				OutputDebugStringA(std::format("None {} {}\n", x, y).c_str());
#endif
			}
		}
		RoomLocalRects.Add(BlockRect);
	}
}

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
void MazeGenerator::MakePath(const UINT8& x, const UINT8& y, const FBlock& block)
{
	if (!IsInBlock(x, y))
	{
		return;
	}
	const FRect& roomRect = RoomLocalRects[y][x];

	UINT8 currSx, currSy, nextSx, nextSy;
	if (JOINED_BLOCK(block.JoinR))
	{
		if (IsInBlock(x + 1, y))
		{
#if _DEBUG
			//OutputDebugStringA("----------------------------------------------------------------------\n");
			//OutputDebugStringA(std::format("Make Path JoinR {} {} to {} {}\n", x, y, x + 1, y).c_str());
#endif

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


#if _DEBUG
			//OutputDebugStringA(std::format("Make Path X {} {} {}\n", currSx, borderX, currSy).c_str());
			//OutputDebugStringA(std::format("Make Path X {} {} {}\n", borderX, nextSx, nextSy).c_str());
#endif

			// Merge Path
			if (currSy > nextSy)
			{
				swap(currSy, nextSy);
			}
			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), currSy, nextSy, borderX);

#if _DEBUG
			//OutputDebugStringA(std::format("Merge Path V {} {} {}\n", currSy, nextSy, borderX).c_str());
			//OutputDebugStringA("----------------------------------------------------------------------\n");
#endif
		}
	}
	if (JOINED_BLOCK(block.JoinD))
	{
		if (IsInBlock(x, y + 1))
		{
#if _DEBUG
			//OutputDebugStringA("----------------------------------------------------------------------\n");
			//OutputDebugStringA(std::format("Make Path JoinD {} {} to {} {}\n", x, y, x, y + 1).c_str());
#endif

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

#if _DEBUG
			//OutputDebugStringA(std::format("Make Path Y {} {} {}\n", currSy, borderY, currSx).c_str());
			//OutputDebugStringA(std::format("Make Path Y {} {} {}\n", borderY, nextSy, nextSx).c_str());
#endif

			// Merge Path
			if (currSx > nextSx)
			{
				swap(currSx, nextSx);
			}
			SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), currSx, nextSx, borderY, true);

#if _DEBUG
			//OutputDebugStringA(std::format("Merge Path H {} {} {}\n", currSx, nextSx, borderY).c_str());
			//OutputDebugStringA("----------------------------------------------------------------------\n");
#endif
		}
	}
}
void MazeGenerator::GetPathStartPos(const FRect& inRect, const EDirection& direction, UINT8& x, UINT8& y)
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

// --------------------------
// Main : Ground Layer : Utils
// --------------------------
bool MazeGenerator::IsInMaze(const UINT8& x, const UINT8& y) const noexcept
{
	return (x >= 0 && x < actualMazeCountX) && (y >= 0 && y < actualMazeCountY);
}
bool MazeGenerator::IsInBlock(const UINT8& x, const UINT8& y) const noexcept
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

void MazeGenerator::GetMazeXY(const UINT16& pos, UINT8& x, UINT8& y) const noexcept
{
	// 28 % 5 = 3
	x = (UINT8)(pos % actualMazeCountX);
	// 28 / 5 = 5
	y = (UINT8)(pos / actualMazeCountX);
}
void MazeGenerator::GetBlockXY(const UINT16& pos, UINT8& x, UINT8& y) const noexcept
{
	x = (UINT8)(pos % actualBlockCountX);
	y = (UINT8)(pos / actualBlockCountX);
}

UINT16 MazeGenerator::GetPosMaze(const UINT8& x, const UINT8& y) const noexcept
{
	return y * actualMazeCountX + x;
}
UINT16 MazeGenerator::GetPosBlock(const UINT8& x, const UINT8& y) const noexcept
{
	return y * actualBlockCountX + x;
}

UINT16 MazeGenerator::BlockToMaze(const UINT8& blockXorY, const UINT8& offsetXorY) const noexcept
{
	return blockXorY * GameSettings::BLOCK_SIZE + offsetXorY;
}
UINT8 MazeGenerator::MazeToBlock(const UINT8& worldXorY, const UINT8& blockXorY) const noexcept
{
	return worldXorY / GameSettings::BLOCK_SIZE - blockXorY;
}

// --------------------------
// Main : Ground Layer : Debug
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

// ------------------------------------------------------
// Main : Event Layer
// ------------------------------------------------------
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
void MazeGenerator::SetEnter(const UINT8& blockX, const UINT8& blockY)
{
	const FRect& localRect = RoomLocalRects[blockY][blockX];
	UINT16 resX = BlockToMaze(blockX, RandRange(localRect.left, localRect.right));
	UINT16 resY = BlockToMaze(blockY, RandRange(localRect.top, localRect.bottom));
	if (CheckIsEnter(blockIDs[blockY][blockX].id, resX, resY))
	{
		SetEnter(blockX, blockY);
	}
	else
	{
		SetEventLayerID(EEventId::Enter, resX, resY);
		SetStartPosition(FVector(resX, 0, resY));
	}
}
void MazeGenerator::SetExit(const UINT8& blockX, const UINT8& blockY)
{
	const FRect& localRect = RoomLocalRects[blockY][blockX];
	UINT16 resX = BlockToMaze(blockX, RandRange(localRect.left, localRect.right));
	UINT16 resY = BlockToMaze(blockY, RandRange(localRect.top, localRect.bottom));

	pExit = GetWorld()->SpawnActor<Event_DungeonExit>(*pDX);
	pExit->SetOuter(shared_from_this());
	SetEventLayerID(pExit, resX, resY);

	pExit->OnChoiceYes.Bind<&MazeGenerator::StartMoveToNextFloor>(*this, "MazeExit");
}

void MazeGenerator::StartMoveToNextFloor()
{
	if (pExit)
	{
		pExit->OnChoiceYes.Unbind("MazeExit");
	}

	GetWorld()->GetGameState()->OnLandmarkClosed.Bind<&MazeGenerator::CompletedMoveToNextFloor>(*this, "MazeGenerator");
	GetWorld()->GetGameState()->OpenLandmarkUI(*pDX, L"", 0.5f);
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
	UINT8 resX, resY;
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
