#pragma once

#include "DirectX/DirectXHead.h"

class Config
{
public:
	Config() = default;

	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

	// -------------------------------------------------
	// Settings : Application
	// -------------------------------------------------
	static const DirectX::XMFLOAT2 windowSize;
	static const std::wstring windowTitle;

	// -------------------------------------------------
	// Settings : Game
	// -------------------------------------------------
	static const float CELL;
	static const int   INDEX_NONE;
	static const UINT8 EMPTY_PUYO;

	static const DirectX::XMFLOAT2 GAMESCREEN_BACKGROUND_SIZE;
	static const DirectX::XMFLOAT2 GAMESCREEN_PADDING;
	static const DirectX::XMFLOAT2 GAMESCREEN_SIZE;
	static const UINT8 GAMESCREEN_WIDTH;
	static const UINT8 GAMESCREEN_HEIGHT;

	static const DirectX::XMFLOAT2 GAMEOVER_COORD;

	static const DWORD PUYO_FALL_SPEED_GAMESTATE_PRESS_DOWN;
	static const DWORD PUYO_FALL_SPEED_GAMESTATE_CONTROL;
	static const DWORD PUYO_FALL_SPEED_GAMESTATE_RELEASE;
	static const DWORD PUYO_FALL_SPEED_GAMESTATE_VANISH;
	static const DWORD PUYO_FALL_SPEED_GAMESTATE_VANISH_TO_FALL;
	static const DWORD PUYO_FALL_SPEED_GAMESTATE_WAIT;

	static const DWORD PUTO_EFFECT_TIME_VANISH;
	static const std::vector<int> comboBonus;
	static const std::vector<int> connectBonus;
	static const std::vector<int> colorBonus;

	static const std::wstring PuyoImages[6];

	// -------------------------------------------------
	// Settings : Game UI
	// -------------------------------------------------
	static const DirectX::XMFLOAT2 GAMEUI_SIZE;
	static const DirectX::XMFLOAT2 GAMEUI_LEFT_TOP;

	static const DirectX::XMFLOAT2 GAMEUI_RIGHT_BOTTOM;

	// -------------------------------------------------
	// Settings : Game UI Next Puyo
	// -------------------------------------------------
	static const DirectX::XMFLOAT2 GAMEUI_NEXT_PUYO_LEFT_TOP;
};