
#include "GameSettings.h"

// -------------------------------------------------
// GameSettings : Game
// -------------------------------------------------
const float GameSettings::CELL = 64.f;
const int   GameSettings::INDEX_NONE = -1;
const DirectX::XMFLOAT2 GameSettings::GAMESCREEN_BACKGROUND_SIZE = { 768.f, 512.f };
const DirectX::XMFLOAT2 GameSettings::GAMESCREEN_PADDING = { 16.f, 32.f };
/* 416 x  */
const DirectX::XMFLOAT2 GameSettings::GAMESCREEN_SIZE = {
	GAMESCREEN_BACKGROUND_SIZE.x + GAMESCREEN_PADDING.x * 2.f,
	GAMESCREEN_BACKGROUND_SIZE.y + GAMESCREEN_PADDING.y * 2.f
};
const UINT8 GameSettings::GAMESCREEN_WIDTH = 20;
const UINT8 GameSettings::GAMESCREEN_HEIGHT = 12;

const UINT8 GameSettings::BLOCK_SIZE = 8;

const UINT8 GameSettings::BLOCK_MIN_COUNT = 3;
const UINT8 GameSettings::BLOCK_MAX_COUNT_X = 5;
const UINT8 GameSettings::BLOCK_MAX_COUNT_Y = 5;



// -------------------------------------------------
// GameSettings : Game UI
// -------------------------------------------------
const DirectX::XMFLOAT2 GameSettings::GAMEUI_SIZE = {
	300.f, 726.f
};
const DirectX::XMFLOAT2 GameSettings::GAMEUI_LEFT_TOP = {
	GAMESCREEN_SIZE.x + GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y
};

const DirectX::XMFLOAT2 GameSettings::GAMEUI_RIGHT_BOTTOM = {
	GAMESCREEN_SIZE.x + GAMEUI_SIZE.x,
	GAMESCREEN_SIZE.y + GAMEUI_SIZE.y - GAMESCREEN_PADDING.y
};
