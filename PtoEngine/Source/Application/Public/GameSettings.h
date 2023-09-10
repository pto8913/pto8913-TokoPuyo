#pragma once

#include "Math/Vector.h"
#include "Actor/Character/PuyoTypes.h"

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
	static const int   INDEX_NONE;
	static const uint8_t EMPTY_PUYO;

	static const FVector2D GAMESCREEN_PADDING;

	static FVector2D GetGameBoardSize();
	static void SetGameBoardSize(const FVector2D& in);

	static FVector2D GetGameOverCoord();
	static void SetGameOverCoord(const FVector2D& in);

	static int GetPuyoFallSpeed(const EPuyoControl& in);
	static void SetPuyoFallSpeed(const EPuyoControl& type, const int& in);

	static int GetPuyoEffectTime(const EPuyoControl& in);
	static void SetPuyoEffectTime(const EPuyoControl& type, const int& in);

	static int GetBonusCombo(int comboCount);
	static void SetBonusCombo(const std::vector<int>& in);

	static int GetBonusConnect(int connectCount);
	static void SetBonusConnect(const std::vector<int>& in);

	static int GetBonusColor(int colorCount);
	static void SetBonusColor(const std::vector<int>& in);

	static std::wstring GetPuyoImage(int id);
	static std::wstring GetPuyoGameMode(int id);

	// -------------------------------------------------
	// Settings : Game UI
	// -------------------------------------------------
	static const FVector2D GAMEUI_SIZE;

};