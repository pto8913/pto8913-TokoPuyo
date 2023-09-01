
#include "GameSettings.h"
#include "AppSettings.h"

// -------------------------------------------------
// GameSettings : Game
// -------------------------------------------------

const int          GameSettings::INDEX_NONE = -1;

const FVector2D GameSettings::GAMESCREEN_PADDING = { 16.f, 16.f };

const FVector2D GameSettings::GAMESCREEN_LEFT_TOP = {
	GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y ,
};

const float GameSettings::GAMESCREEN_SIZE_WIDTH = AppSettings::GetWindowSize().x - GAMESCREEN_PADDING.x * 2.f;
const float GameSettings::GAMESCREEN_SIZE_HEIGHT = AppSettings::GetWindowSize().y - GAMESCREEN_PADDING.y * 2.f;

const UINT8 GameSettings::BLOCK_SIZE = 9;

const UINT8 GameSettings::BLOCK_MIN_COUNT = 3;
const UINT8 GameSettings::BLOCK_MAX_COUNT_X = 5;
const UINT8 GameSettings::BLOCK_MAX_COUNT_Y = 5;

// -------------------------------------------------
// GameSettings : Game UI
// -------------------------------------------------
const FVector2D GameSettings::MESSAGEBOX_LEFT_TOP = {
	GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y * 2 + AppSettings::GetWindowSize().y / 2.f,
};

const FVector2D GameSettings::GAMEUI_SIZE = {
	300.f, 726.f
};

const FVector2D GameSettings::GAMEUI_LEFT_TOP = {
	GAMESCREEN_PADDING.x * 2 + AppSettings::GetWindowSize().x / 1.5f,
	GAMESCREEN_PADDING.y,
};

const FVector2D GameSettings::GAMEUI_RIGHT_BOTTOM = {
	GAMESCREEN_SIZE_WIDTH + GAMEUI_SIZE.x,
	GAMESCREEN_SIZE_HEIGHT + GAMEUI_SIZE.y - GAMESCREEN_PADDING.y
};
