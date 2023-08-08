#pragma once

#include "DirectX/DirectXHead.h"

#include "Algorithm/Array.h"
#include <memory>

class Sprite;

enum class ETileID
{
	Floor,
	Room,
	Wall,
	Path,
	PathCross,
	Exit,
	Border,
	RoomWall,
};

enum class EDirection
{
	Left,
	Right,
	Up,
	Down,
	InValid
};

class World2D
{
public:
	World2D& operator=(World2D& Other);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void Init(const UINT16& x, const UINT16& y);
	void Clear();

	void DoFrame(float deltaTime);

	// --------------------------
	// Main : Utils
	// --------------------------
	UINT16 GetSize() const noexcept;

	bool IsInScreen(const UINT8& x, const UINT8& y) const noexcept;
	bool IsInWorld(const UINT16& x, const UINT16& y) const noexcept;
	
	void SetTileID(const ETileID& NewTileID, const UINT16& x, const UINT16& y);
	void SetTileID(const ETileID& NewTileID, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal = false, INT16 inConstantXY2 = -1);

	// --------------------------
	// Main : Debug
	// --------------------------
	void ShowTiles();

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	TArray<TArray<ETileID>> Tiles;
	TArray<TArray<std::shared_ptr<Sprite>>> Sprites;

private:
	TArray<UINT16> InScreenTileIndices;

	//// load sprite screen x + 1, x - 1, y + 1, y - 1
	//const UINT8 bufferSize = 1;

	UINT16 width = 0;
	UINT16 height = 0;
};