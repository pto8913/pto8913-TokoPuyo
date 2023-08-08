#pragma once

#include "DirectX/DirectXHead.h"

class GameSettings
{
public:
	GameSettings() = default;

	GameSettings(const GameSettings&) = delete;
	GameSettings& operator=(const GameSettings&) = delete;

	// -------------------------------------------------
	// Settings : Game
	// -------------------------------------------------
	static const DirectX::XMFLOAT2 GAMESCREEN_SIZE;
	static const float CELL;
	static const int   INDEX_NONE;

	static const DirectX::XMFLOAT2 GAMESCREEN_BACKGROUND_SIZE;
	static const DirectX::XMFLOAT2 GAMESCREEN_PADDING;
	static const DirectX::XMFLOAT2 GAMESCREEN_SIZE;
	static const UINT8 GAMESCREEN_WIDTH;
	static const UINT8 GAMESCREEN_HEIGHT;

	static const UINT8 BLOCK_SIZE;
	static const UINT8 BLOCK_MIN_COUNT;
	static const UINT8 BLOCK_MAX_COUNT_X;
	static const UINT8 BLOCK_MAX_COUNT_Y;

	// -------------------------------------------------
	// Settings : Game UI
	// -------------------------------------------------
	static const DirectX::XMFLOAT2 GAMEUI_SIZE;
	static const DirectX::XMFLOAT2 GAMEUI_LEFT_TOP;

	static const DirectX::XMFLOAT2 GAMEUI_RIGHT_BOTTOM;
};