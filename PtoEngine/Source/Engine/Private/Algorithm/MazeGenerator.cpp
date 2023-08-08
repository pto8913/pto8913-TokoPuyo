
#include "Algorithm/MazeGenerator.h"
#include "Algorithm/algo.h"

#include "GameSettings.h"

#include "Math/Math.h"

#include "Object/Sprite.h"

#include <format>

using namespace DirectX;
using namespace Math;
using namespace Algo;

#define _DEBUG 1

#define JOINED_BLOCK(x) x == EBlockJoinState::Joined

// ------------------------------------------------------
// Main
// ------------------------------------------------------

std::shared_ptr<World2D> MazeGenerator::GenerateMaze()
{
	mWorld = std::make_shared<World2D>();

	ClearMaze();

	InitializeMaze();
	InitializeBlock();

	mWorld->ShowTiles();

	ShowBlock();

	return mWorld;
}

// --------------------------
// Main : Initialization
// --------------------------
void MazeGenerator::ClearMaze()
{
	mWorld->Clear();
	blockIDs.Clear();
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

	mWorld->Init(actualMazeCountX, actualMazeCountY);
}

// --------------------------
// Main : Block Initialization
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

	MakeRoom();

	MakePath();
}

void MazeGenerator::JoinBlock()
{
	const int sx = Random(actualBlockCountX - 1);
	const int sy = Random(actualBlockCountY - 1);

	if (blockIDs[sy][sx].id == EBlockID::None)
	{
		JoinBlock();
		return;
	}

	TArray<EDirection> JoinDirections;
	GetJoinableDirection(JoinDirections, sx, sy);
	JoinBlock((UINT8)sx, (UINT8)sy, JoinDirections.RandomValue());

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
void MazeGenerator::JoinBlock(const UINT8& x, const UINT8& y, const EDirection& direction)
{
	switch (direction)
	{
	case EDirection::Left:
		blockIDs[y][x].JoinL = EBlockJoinState::Joined;
		blockIDs[y][x - 1].JoinR = EBlockJoinState::Joined;
		break;
	case EDirection::Right:
		blockIDs[y][x].JoinR = EBlockJoinState::Joined;
		blockIDs[y][x + 1].JoinL = EBlockJoinState::Joined;
		break;
	case EDirection::Up:
		blockIDs[y][x].JoinU = EBlockJoinState::Joined;
		blockIDs[y - 1][x].JoinD = EBlockJoinState::Joined;
		break;
	case EDirection::Down:
		blockIDs[y][x].JoinD = EBlockJoinState::Joined;
		blockIDs[y + 1][x].JoinU = EBlockJoinState::Joined;
		break;
	default:
		break;
	}
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

	if (candidateJoinBlock.Size() > 2)
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
			JoinBlock(x, y, JoinableDirection.RandomValue());

			candidateJoinBlock.Remove(candidateIdx);
			JoinableDirection.Clear();
		}
	}
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

				const UINT16 Sy = LocalToWorld(y, actualRoomLeftTopY);
				const UINT16 Ey = LocalToWorld(y, actualRoomRightBottomY);

				const UINT16 Sx = LocalToWorld(x, actualRoomLeftTopX);
				const UINT16 Ex = LocalToWorld(x, actualRoomRightBottomX);

				for (int constantY = Sy; constantY <= Ey; ++constantY)
				{
					mWorld->SetTileID(ETileID::Room, Sx, Ex, constantY, true);
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
				const UINT16 Sx = LocalToWorld(x, rectX);
				const UINT16 Sy = LocalToWorld(y, rectY);

				mWorld->SetTileID(ETileID::PathCross, Sx, Sy);
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
			OutputDebugStringA("----------------------------------------------------------------------\n");
			OutputDebugStringA(std::format("Make Path JoinR {} {} to {} {}\n", x, y, x + 1, y).c_str());
#endif

			GetPathStartPos(roomRect, EDirection::Right, currSx, currSy);

			const FBlock& nextBlock = blockIDs[y][x + 1];
			const FRect& nextRoomRect = RoomLocalRects[y][x + 1];
			GetPathStartPos(nextRoomRect, EDirection::Left, nextSx, nextSy);

			currSx = LocalToWorld(x, currSx);
			currSy = LocalToWorld(y, currSy);

			nextSx = LocalToWorld(x + 1, nextSx);
			nextSy = LocalToWorld(y, nextSy);

			int borderX = RandRange(currSx + 1, nextSx - 1);

			mWorld->SetTileID(ETileID::Path, currSx, borderX, currSy, true);
			mWorld->SetTileID(ETileID::Path, borderX, nextSx, nextSy, true);

#if _DEBUG
			OutputDebugStringA(std::format("Make Path X {} {} {}\n", currSx, borderX, currSy).c_str());
			OutputDebugStringA(std::format("Make Path X {} {} {}\n", borderX, nextSx, nextSy).c_str());
#endif

			// Merge Path
			if (currSy > nextSy)
			{
				swap(currSy, nextSy);
			}
			mWorld->SetTileID(ETileID::Path, currSy, nextSy, borderX);

#if _DEBUG
			OutputDebugStringA(std::format("Merge Path V {} {} {}\n", currSy, nextSy, borderX).c_str());
			OutputDebugStringA("----------------------------------------------------------------------\n");
#endif
		}
	}
	if (JOINED_BLOCK(block.JoinD))
	{
		if (IsInBlock(x, y + 1))
		{
#if _DEBUG
			OutputDebugStringA("----------------------------------------------------------------------\n");
			OutputDebugStringA(std::format("Make Path JoinD {} {} to {} {}\n", x, y, x, y + 1).c_str());
#endif

			GetPathStartPos(roomRect, EDirection::Down, currSx, currSy);

			const FBlock& nextBlock = blockIDs[y + 1][x];
			const FRect& nextRoomRect = RoomLocalRects[y + 1][x];
			GetPathStartPos(nextRoomRect, EDirection::Up, nextSx, nextSy);

			currSx = LocalToWorld(x, currSx);
			currSy = LocalToWorld(y, currSy);

			nextSx = LocalToWorld(x, nextSx);
			nextSy = LocalToWorld(y + 1, nextSy);

			int borderY = RandRange(currSy + 1, nextSy - 1);

			mWorld->SetTileID(ETileID::Path, currSy, borderY, currSx);
			mWorld->SetTileID(ETileID::Path, borderY, nextSy, nextSx);

#if _DEBUG
			OutputDebugStringA(std::format("Make Path Y {} {} {}\n", currSy, borderY, currSx).c_str());
			OutputDebugStringA(std::format("Make Path Y {} {} {}\n", borderY, nextSy, nextSx).c_str());
#endif

			// Merge Path
			if (currSx > nextSx)
			{
				swap(currSx, nextSx);
			}
			mWorld->SetTileID(ETileID::Path, currSx, nextSx, borderY, true);

#if _DEBUG
			OutputDebugStringA(std::format("Merge Path H {} {} {}\n", currSx, nextSx, borderY).c_str());
			OutputDebugStringA("----------------------------------------------------------------------\n");
#endif
		}
	}
}
void MazeGenerator::GetPathStartPos(const FRect& inRect, const EDirection& direction, UINT8& x, UINT8& y) const noexcept
{
	TArray<int> candidatePathStartPos;
	switch (direction)
	{
	case EDirection::Right:
		for (int roomY = inRect.t; roomY <= inRect.b; ++roomY)
		{
			candidatePathStartPos.Add(GetPosMaze(inRect.r + 1, roomY));
		}
		break;
	case EDirection::Left:
		for (int roomY = inRect.t; roomY <= inRect.b; ++roomY)
		{
			candidatePathStartPos.Add(GetPosMaze(inRect.l - 1, roomY));
		}
		break;
	case EDirection::Up:
		for (int roomX = inRect.l; roomX <= inRect.r; ++roomX)
		{
			candidatePathStartPos.Add(GetPosMaze(roomX, inRect.t - 1));
		}
		break;
	case EDirection::Down:
		for (int roomX = inRect.l; roomX <= inRect.r; ++roomX)
		{
			candidatePathStartPos.Add(GetPosMaze(roomX, inRect.b + 1));
		}
		break;
	default:
		break;
	}

	GetMazeXY(candidatePathStartPos.RandomValue(), x, y);
}

// --------------------------
// MazeGenerator : Utils
// --------------------------
bool MazeGenerator::IsInMaze(const UINT8& x, const UINT8& y) const noexcept
{
	return (x >= 0 && x < actualMazeCountX) && (y >= 0 && y < actualMazeCountY);
}
bool MazeGenerator::IsInBlock(const UINT8& x, const UINT8& y) const noexcept
{
	return (x >= 0 && x < actualBlockCountX) && (y >= 0 && y < actualBlockCountY);
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

UINT16 MazeGenerator::LocalToWorld(const UINT8& blockXorY, const UINT8& offsetXorY) const noexcept
{
	return blockXorY * GameSettings::BLOCK_SIZE + offsetXorY;
}
UINT8 MazeGenerator::WorldToLocal(const UINT8& worldXorY, const UINT8& blockXorY) const noexcept
{
	return worldXorY / GameSettings::BLOCK_SIZE - blockXorY;
}

// --------------------------
// MazeGenerator : Debug
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