#pragma once

/*
* LocalCoord : on block coordinate.
* ex. This is the block List
* 
* CRCRR
* C CCC
* RCR C
*
* ex. This is details of the one block.
*
* 					     ########
* 					     ########
* 					     #0######
* CRCRR                  ########
* C CCC => RCC C => R => ####1###
* RCC C                  ########
* 					     ########
* 					     ########
*
* block coord : (x, y) = (0, 2)
*
* 0 local coord : (x, y) = (1, 2)
* 1 local coord : (x, y) = (4, 4)
*
* WorldCoord : on World2D coordinate.
*
* 0 local coord : (x, y) = (1, 2)
* 1 local coord : (x, y) = (4, 4)
*
* 0 to world coord : (x, y) = (0 * BLOCK_SIZE + 1, 2 * BLOCK_SIZE + 2)
* 1 to world coord : (x, y) = (0 * BLOCK_SIZE + 4, 2 * BLOCK_SIZE + 4)
*
*
*/

#include "DirectX/DirectXHead.h"

#include "Algorithm/UnionFind.h"
#include "Algorithm/Array.h"
#include "Algorithm/Rect.h"

#include "Object/World.h"

/*

*/
class MazeGenerator
{
public:
	MazeGenerator() = default;

private:
	// ------------------------------------------------------
	// Sub Structure
	// ------------------------------------------------------

	// --------------------------
	// Sub Structure : Block
	// --------------------------
	class FBlock
	{
	public:
		enum class EBlockID
		{
			None,
			Room,
			Path,
		};

		enum class EBlockJoinState
		{
			None,
			Joined,
			OutOfRange,
		};

		FBlock(EBlockID inID);
		
		EBlockID id;

		EBlockJoinState JoinL;
		EBlockJoinState JoinR;
		EBlockJoinState JoinU;
		EBlockJoinState JoinD;

		bool IsJoinedAll() const noexcept;
		bool IsJoinedAtLeastTwo() const noexcept;
		bool IsJoinedOne() const noexcept;
		void GetJoinableDirection(TArray<EDirection>& Out) const noexcept;
	};
	using FBlock = MazeGenerator::FBlock;
	using EBlockID = FBlock::EBlockID;
	using EBlockJoinState = FBlock::EBlockJoinState;

public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	std::shared_ptr<World2D> GenerateMaze();
private:
	// --------------------------
	// Main : Initialization
	// --------------------------
	void ClearMaze();
	void InitializeMaze();

	// --------------------------
	// Main : Block Initialization
	// --------------------------
	void InitializeBlock();
	
	void JoinBlock();
	void JoinBlock(const UINT8& x, const UINT8& y, const EDirection& direction);
	void GetJoinableDirection(TArray<EDirection>& out, const UINT8& sx, const UINT8& sy);
	void JoinBlockAdditional();

	void MakeRoom();

	void MakePath();
	void MakePath(const UINT8& x, const UINT8& y, const FBlock& block);
	void GetPathStartPos(const FRect& inRect, const EDirection& direction, UINT8& x, UINT8& y) const noexcept;

	// --------------------------
	// Main : Utils
	// --------------------------

	bool IsInMaze(const UINT8& x, const UINT8& y) const noexcept;
	bool IsInBlock(const UINT8& x, const UINT8& y) const noexcept;

	void GetMazeXY(const UINT16& pos, UINT8& x, UINT8& y) const noexcept;
	void GetBlockXY(const UINT16& pos, UINT8& x, UINT8& y) const noexcept;

	UINT16 GetPosMaze(const UINT8& x, const UINT8& y) const noexcept;
	UINT16 GetPosBlock(const UINT8& x, const UINT8& y) const noexcept;

	UINT16 LocalToWorld(const UINT8& blockXorY, const UINT8& offsetXorY = 0) const noexcept;
	UINT8 WorldToLocal(const UINT8& worldXorY, const UINT8& blockXorY) const noexcept;

	// --------------------------
	// Main : Debug
	// --------------------------
	void ShowBlock();

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	
	// --------------------------
	// State : Maze
	// --------------------------
	UINT8 actualBlockCountX = 0;
	UINT8 actualBlockCountY = 0;

	UINT16 actualBlockCount = 0;

	UINT16 actualMazeCountX = 0;
	UINT16 actualMazeCountY = 0;

	UINT16 actualMazeCount = 0;

	TArray<TArray<FBlock>> blockIDs;
	UnionFind blockUnionFind;

	std::shared_ptr<World2D> mWorld;
	TArray<TArray<FRect>> RoomLocalRects;
};