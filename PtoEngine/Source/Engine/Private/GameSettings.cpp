
#include "GameSettings.h"
#include "Core/AppSettings.h"

// -------------------------------------------------
// GameSettings : Game
// -------------------------------------------------
const float        GameSettings::CELL = 82.f;
const int          GameSettings::INDEX_NONE = -1;

const FVector2D GameSettings::GAMESCREEN_PADDING = { 16.f, 16.f };

const FVector2D GameSettings::GAMESCREEN_LEFT_TOP = {
	GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y ,
};

const float GameSettings::GAMESCREEN_SIZE_WIDTH = AppSettings::windowSize.x - GAMESCREEN_PADDING.x * 2.f;
const float GameSettings::GAMESCREEN_SIZE_HEIGHT = AppSettings::windowSize.y - GAMESCREEN_PADDING.y * 2.f;

const UINT8 GameSettings::GAMESCREEN_WIDTH = 9;
const UINT8 GameSettings::GAMESCREEN_HEIGHT = 7;

const UINT8 GameSettings::GAMESCREEN_CENTER_X = GameSettings::GAMESCREEN_WIDTH / 2 - 1;
const UINT8 GameSettings::GAMESCREEN_CENTER_Y = GameSettings::GAMESCREEN_HEIGHT / 2 - 1;

const UINT8 GameSettings::BLOCK_SIZE = 9;

const UINT8 GameSettings::BLOCK_MIN_COUNT = 3;
const UINT8 GameSettings::BLOCK_MAX_COUNT_X = 5;
const UINT8 GameSettings::BLOCK_MAX_COUNT_Y = 5;

// -------------------------------------------------
// GameSettings : Game UI
// -------------------------------------------------
const FVector2D GameSettings::MESSAGEBOX_LEFT_TOP = {
	GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y * 2 + AppSettings::windowSize.y / 2.f,
};

const FVector2D GameSettings::GAMEUI_SIZE = {
	300.f, 726.f
};

const FVector2D GameSettings::GAMEUI_LEFT_TOP = {
	GAMESCREEN_PADDING.x * 2 + AppSettings::windowSize.x / 1.5f,
	GAMESCREEN_PADDING.y,
};

const FVector2D GameSettings::GAMEUI_RIGHT_BOTTOM = {
	GAMESCREEN_SIZE_WIDTH + GAMEUI_SIZE.x,
	GAMESCREEN_SIZE_HEIGHT + GAMEUI_SIZE.y - GAMESCREEN_PADDING.y
};
