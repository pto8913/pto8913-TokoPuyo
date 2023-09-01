#pragma once

#include "DirectX/DirectXHead.h"
#include "Math/Vector.h"

class GameSettings
{
public:
	GameSettings() = default;

	GameSettings(const GameSettings&) = delete;
	GameSettings& operator=(const GameSettings&) = delete;

	// -------------------------------------------------
	// Settings : Game
	// -------------------------------------------------
	static const float CELL;
	static const FVector2D GetCELL();
	static const FVector2D GET_CELL(const float& x, const float& y);
	static const int   INDEX_NONE;

	static const FVector2D GAMESCREEN_LEFT_TOP;

	static const FVector2D GAMESCREEN_PADDING;

	static const float GAMESCREEN_SIZE_WIDTH;
	static const float GAMESCREEN_SIZE_HEIGHT;

	static const UINT8 BLOCK_SIZE;
	static const UINT8 BLOCK_MIN_COUNT;
	static const UINT8 BLOCK_MAX_COUNT_X;
	static const UINT8 BLOCK_MAX_COUNT_Y;

	// -------------------------------------------------
	// Settings : Game UI
	// -------------------------------------------------
	static const FVector2D MESSAGEBOX_LEFT_TOP;

	static const FVector2D GAMEUI_SIZE;
	static const FVector2D GAMEUI_LEFT_TOP;
	static const FVector2D GAMEUI_RIGHT_BOTTOM;
};