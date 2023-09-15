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

#include "Level2D.h"

#include "DirectX/DirectXHead.h"

#include "Algorithm/UnionFind.h"
#include "Algorithm/Array.h"
#include "Math/Rect.h"
#include "Math/Vector.h"

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
	virtual std::string GetName() const override
	{
		return "MazeGenerator_" + std::to_string(mID);
	}
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
	bool JoinBlock(const uint16_t& x, const uint16_t& y, const EDirection& direction);
	void GetJoinableDirection(TArray<EDirection>& out, const uint16_t& sx, const uint16_t& sy);
	void JoinBlockAdditional();
	bool JoinClosedBlock();

	// --------------------------
	// Main : Ground Layer : Room
	// --------------------------
	void MakeRoom();

	// --------------------------
	// Main : Ground Layer : Path
	// --------------------------
	void MakePath();
	void MakePath(const uint16_t& x, const uint16_t& y, const FBlock& block);
	void GetPathStartPos(const FRect& inRect, const EDirection& direction, uint16_t& x, uint16_t& y);

	// --------------------------
	// Main : Ground Layer : Wall
	// --------------------------
	void MakeWall();
	void MakeWall(const int& x, const int& y, const int& addX, const int& addY);

	// ------------------------------------------------------
	// Main : Event Layer
	// ------------------------------------------------------
	void SetEnterBlockEvent();
	void SetEnterBlockEvent(const int& worldX, const int& worldY);
	void CheckSetEnterBlockCount(const int& blockX, const int& blockY);

	void SetEnterExit();
	void SetEnter(const uint16_t& blockX, const uint16_t& blockY);
	void SetExit(const uint16_t& blockX, const uint16_t& blockY);

	void StartMoveToNextFloor();
	void CompletedMoveToNextFloor();

	// ------------------------------------------------------
	// Main : Item Layer
	// ------------------------------------------------------
	void SpawnItems();

	// --------------------------
	// Main : Debug
	// --------------------------
	void ShowBlock();

	// --------------------------
	// Main : Ground Layer : Utils
	// --------------------------
	virtual std::shared_ptr<GroundBase> SetGroundLayerIDSpe(const EGroundId& groundType, const float& worldX, const float& worldY) override;

	bool IsInMaze(const uint16_t& x, const uint16_t& y) const noexcept;
	bool IsInBlock(const uint16_t& x, const uint16_t& y) const noexcept;

	bool CheckIsEnter(const EBlockID& blockID, const int& worldX, const int& worldY) const noexcept;

	void GetMazeXY(const uint16_t& pos, uint16_t& x, uint16_t& y) const noexcept;
	void GetBlockXY(const uint16_t& pos, uint16_t& x, uint16_t& y) const noexcept;

	uint16_t GetPosMaze(const uint16_t& x, const uint16_t& y) const noexcept;
	uint16_t GetPosBlock(const uint16_t& x, const uint16_t& y) const noexcept;

	uint16_t BlockToMaze(const uint16_t& blockXorY, const uint16_t& offsetXorY = 0) const noexcept;
	uint16_t MazeToBlock(const uint16_t& worldXorY, const uint16_t& blockXorY) const noexcept;

	// ------------------------------------------------------
	// Main : Delegate
	// ------------------------------------------------------
	FOnDungeonNextFloor OnDungeonNextFloor;
	FOnEnterBlock OnEnterBlock;

	// --------------------------
	// Settings : Item Layer
	// --------------------------
	uint16_t itemCount = 6;

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------

	// --------------------------
	// State : Ground Layer 
	// --------------------------
	uint16_t actualBlockCountX = 0;
	uint16_t actualBlockCountY = 0;

	uint16_t actualBlockCount = 0;

	uint16_t actualMazeCountX = 0;
	uint16_t actualMazeCountY = 0;

	uint16_t actualMazeCount = 0;

	TArray<TArray<FBlock>> blockIDs;
	UnionFind blockUnionFind;

	TArray<TArray<FRect>> RoomLocalRects;
	uint16_t actualRoomCount = 0;

	TArray<TArray<EGroundTile>> mazeTils;

	// --------------------------
	// State : Event Layer 
	// --------------------------
	std::shared_ptr<Event_DungeonExit> pExit = nullptr;
};
