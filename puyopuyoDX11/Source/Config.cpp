
#include "Config.h"

// -------------------------------------------------
// Settings : Application
// -------------------------------------------------
const DirectX::XMFLOAT2 Config::windowSize = { 768.f, 786.f };
const std::wstring Config::windowTitle = L"pto8913";

// -------------------------------------------------
// Settings : Game
// -------------------------------------------------
const float Config::CELL = 64.f;
const int   Config::INDEX_NONE = -1;
const UINT8 Config::EMPTY_PUYO = 5u;

const DirectX::XMFLOAT2 Config::GAMESCREEN_BACKGROUND_SIZE = { 384.f, 768.f };
const DirectX::XMFLOAT2 Config::GAMESCREEN_PADDING = { 16.f, 32.f };
/* 416 x  */
const DirectX::XMFLOAT2 Config::GAMESCREEN_SIZE = {
	GAMESCREEN_BACKGROUND_SIZE.x + GAMESCREEN_PADDING.x * 2.f,
	GAMESCREEN_BACKGROUND_SIZE.y + GAMESCREEN_PADDING.y * 2.f
};
const UINT8 Config::GAMESCREEN_WIDTH = 6;
const UINT8 Config::GAMESCREEN_HEIGHT = 13;

const DirectX::XMFLOAT2 Config::GAMEOVER_COORD = { 2.f, 1.f };

const DWORD Config::PUYO_FALL_SPEED_GAMESTATE_PRESS_DOWN = 50;
const DWORD Config::PUYO_FALL_SPEED_GAMESTATE_CONTROL = 1000;
const DWORD Config::PUYO_FALL_SPEED_GAMESTATE_RELEASE = 125;
const DWORD Config::PUYO_FALL_SPEED_GAMESTATE_VANISH = 250;
const DWORD Config::PUYO_FALL_SPEED_GAMESTATE_VANISH_TO_FALL = 250;
const DWORD Config::PUYO_FALL_SPEED_GAMESTATE_WAIT = 1000;

const DWORD Config::PUTO_EFFECT_TIME_VANISH = 500;

const std::vector<int> Config::comboBonus = {
	0, 0, 8, 16, 32, 64, 96, 128, 160, 192, 224, 256,
	288, 320, 352, 384, 416, 448, 480, 512, 512, 512, 512
};
const std::vector<int> Config::connectBonus = {
	0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 10, 10, 10, 10, 10, 10, 10, 10
};
const std::vector<int> Config::colorBonus = {
	0, 0, 3, 6, 12, 24
};

const std::wstring Config::PuyoImages[6] = {
	L"Content/Textures/T_Puyo_Red.png",
	L"Content/Textures/T_Puyo_Green.png",
	L"Content/Textures/T_Puyo_Yellow.png",
	L"Content/Textures/T_Puyo_Purple.png",
	L"",
	L""
};

// -------------------------------------------------
// Settings : Game UI
// -------------------------------------------------
const DirectX::XMFLOAT2 Config::GAMEUI_SIZE = {
	300.f, 726.f
};
const DirectX::XMFLOAT2 Config::GAMEUI_LEFT_TOP = {
	GAMESCREEN_SIZE.x + GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y
};

const DirectX::XMFLOAT2 Config::GAMEUI_RIGHT_BOTTOM = {
	GAMESCREEN_SIZE.x + GAMEUI_SIZE.x,
	GAMESCREEN_SIZE.y + GAMEUI_SIZE.y - GAMESCREEN_PADDING.y
};

// -------------------------------------------------
// Settings : Game UI Next Puyo
// -------------------------------------------------
const DirectX::XMFLOAT2 Config::GAMEUI_NEXT_PUYO_LEFT_TOP = {
	GAMEUI_LEFT_TOP.x + GAMESCREEN_PADDING.x * 2,
	GAMEUI_LEFT_TOP.y + GAMESCREEN_PADDING.y
};