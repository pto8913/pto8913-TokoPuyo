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
#pragma once

#include "Level/Level2D.h"

#include "DirectX/DirectXHead.h"

#include "Algorithm/UnionFind.h"
#include "Algorithm/Array.h"
#include "Engine/Rect.h"

class Event_DungeonExit;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDungeonNextFloor, const Level2D*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnterBlock, const FVector2D&);

class MazeGenerator : public Level2D
{
public:
	MazeGenerator(DirectX11& dx);
	virtual ~MazeGenerator();
protected:
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
	virtual void BeginPlay(DirectX11& dx) override;

protected:
	virtual void GenerateGroundLayer() override;
	virtual void GenerateEventLayer() override;
	virtual void GenerateItemLayer() override;
public:
	FOnDungeonNextFloor& GetDungeonNextFloor();
	FOnEnterBlock& GetEnterBlock();

	void EnterBlock(const FEventData& inEventData);
protected:
	// ------------------------------------------------------
	// Main : Ground Layer
	// ------------------------------------------------------
	// --------------------------
	// Main : Ground Layer : Initialization
	// --------------------------
	virtual void Clear() override;

	void InitializeMaze();

	// --------------------------
	// Main : Ground Layer : Block Initialization
	// --------------------------
	void InitializeBlock();

	void JoinBlock();
	bool JoinBlock(const UINT8& x, const UINT8& y, const EDirection& direction);
	void GetJoinableDirection(TArray<EDirection>& out, const UINT8& sx, const UINT8& sy);
	void JoinBlockAdditional();
	bool JoinClosedBlock();

	void MakeRoom();

	void MakePath();
	void MakePath(const UINT8& x, const UINT8& y, const FBlock& block);
	void GetPathStartPos(const FRect& inRect, const EDirection& direction, UINT8& x, UINT8& y);
	void SetEnterBlockEvent();
	void SetEnterBlockEvent(const int& worldX, const int& worldY);
	void CheckSetEnterBlockCount(const int& blockX, const int& blockY);

	// --------------------------
	// Main : Ground Layer : Utils
	// --------------------------

	bool IsInMaze(const UINT8& x, const UINT8& y) const noexcept;
	bool IsInBlock(const UINT8& x, const UINT8& y) const noexcept;

	bool CheckIsEnter(const EBlockID& blockID, const int& worldX, const int& worldY) const noexcept;

	void GetMazeXY(const UINT16& pos, UINT8& x, UINT8& y) const noexcept;
	void GetBlockXY(const UINT16& pos, UINT8& x, UINT8& y) const noexcept;

	UINT16 GetPosMaze(const UINT8& x, const UINT8& y) const noexcept;
	UINT16 GetPosBlock(const UINT8& x, const UINT8& y) const noexcept;

	UINT16 BlockToMaze(const UINT8& blockXorY, const UINT8& offsetXorY = 0) const noexcept;
	UINT8 MazeToBlock(const UINT8& worldXorY, const UINT8& blockXorY) const noexcept;

	// --------------------------
	// Main : Ground Layer : Debug
	// --------------------------
	void ShowBlock();

	// ------------------------------------------------------
	// Main : Event Layer
	// ------------------------------------------------------
	void SetEnterExit();
	void SetEnter(const UINT8& blockX, const UINT8& blockY);
	void SetExit(const UINT8& blockX, const UINT8& blockY);

	void StartMoveToNextFloor();
	void CompletedMoveToNextFloor();

	// ------------------------------------------------------
	// Main : Item Layer
	// ------------------------------------------------------
	void SpawnItems();

	// ------------------------------------------------------
	// Main : Delegate
	// ------------------------------------------------------
	FOnDungeonNextFloor OnDungeonNextFloor;
	FOnEnterBlock OnEnterBlock;

	// --------------------------
	// Settings : Item Layer
	// --------------------------
	UINT8 itemCount = 6;

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------

	// --------------------------
	// State : Ground Layer 
	// --------------------------
	UINT8 actualBlockCountX = 0;
	UINT8 actualBlockCountY = 0;

	UINT16 actualBlockCount = 0;

	UINT16 actualMazeCountX = 0;
	UINT16 actualMazeCountY = 0;

	UINT16 actualMazeCount = 0;

	TArray<TArray<FBlock>> blockIDs;
	UnionFind blockUnionFind;

	TArray<TArray<FRect>> RoomLocalRects;
	UINT8 actualRoomCount = 0;

	// --------------------------
	// State : Event Layer 
	// --------------------------
	std::shared_ptr<Event_DungeonExit> pExit = nullptr;
};
