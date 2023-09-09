
#include "GameSettings.h"
#include "EngineSettings.h"

#include "nlohmann/json.hpp"
#include <fstream>

#include "Utils/String.h"

using namespace nlohmann;

namespace GSSS
{
	json GetJson()
	{
		std::ifstream loadEngineSettings("Config/PuyoGameSettings.json");
		json jsonEngineSettings;
		loadEngineSettings >> jsonEngineSettings;
		return jsonEngineSettings;
	}
	void SaveJson(json inJson)
	{
		std::ofstream saveEngineSettings("Config/PuyoGameSettings.json");
		saveEngineSettings << std::setw(4) << inJson << std::endl;
	}
};

// -------------------------------------------------
// Settings : Game
// -------------------------------------------------
const float GameSettings::CELL = 64.f;
const int   GameSettings::INDEX_NONE = -1;
const uint8_t GameSettings::EMPTY_PUYO = 5u;

const FVector2D GameSettings::GAMESCREEN_PADDING = { 16.f, 16.f };

FVector2D GameSettings::GetGameBoardSize()
{
	json jsonEngineSettings = GSSS::GetJson();

	std::vector<int> v;
	jsonEngineSettings["gameBoardSize"].get_to(v);
	return FVector2D(v[0], v[1]);
}
void GameSettings::SetGameBoardSize(const FVector2D& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	if (jsonEngineSettings.contains("gameBoardSize"))
	{
		jsonEngineSettings["gameBoardSize"] = { in.x, in.y };
	}
	GSSS::SaveJson(jsonEngineSettings);
}

FVector2D GameSettings::GetGameOverCoord()
{
	json jsonEngineSettings = GSSS::GetJson();

	std::vector<int> v;
	jsonEngineSettings["gameOverCoord"].get_to(v);
	return FVector2D(v[0], v[1]);
}
void GameSettings::SetGameOverCoord(const FVector2D& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	if (jsonEngineSettings.contains("gameOverCoord"))
	{
		jsonEngineSettings["gameOverCoord"] = { in.x, in.y };
	}
	GSSS::SaveJson(jsonEngineSettings);
}

int GameSettings::GetPuyoFallSpeed(const EPuyoControl& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	int v;
	switch (in)
	{
	case EPuyoControl::PressDown:
		jsonEngineSettings["puyoFallSpeed_PressDown"].get_to(v);
		break;
	case EPuyoControl::Control:
		jsonEngineSettings["puyoFallSpeed_Control"].get_to(v);
		break;
	case EPuyoControl::Release:
		jsonEngineSettings["puyoFallSpeed_Release"].get_to(v);
		break;
	case EPuyoControl::Vanish:
		jsonEngineSettings["puyoFallSpeed_Vanish"].get_to(v);
		break;
	case EPuyoControl::VanishToFall:
		jsonEngineSettings["puyoFallSpeed_VanishToFall"].get_to(v);
		break;
	default:
		jsonEngineSettings["puyoFallSpeed_Wait"].get_to(v);
		break;
	}
	return v;
}
void GameSettings::SetPuyoFallSpeed(const EPuyoControl& type, const int& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	auto set = [&jsonEngineSettings, &in](std::string id)
	{
		if (jsonEngineSettings.contains(id))
		{
			jsonEngineSettings[id] = in;
		}
	};
	switch (type)
	{
	case EPuyoControl::PressDown:
		set("puyoFallSpeed_PressDown");
		break;
	case EPuyoControl::Control:
		set("puyoFallSpeed_Control");
		break;
	case EPuyoControl::Release:
		set("puyoFallSpeed_Release");
		break;
	case EPuyoControl::Vanish:
		set("puyoFallSpeed_Vanish");
		break;
	case EPuyoControl::VanishToFall:
		set("puyoFallSpeed_VanishToFall");
		break;
	default:
		set("puyoFallSpeed_Wait");
		break;
	}
	GSSS::SaveJson(jsonEngineSettings);
}

int GameSettings::GetPuyoEffectTime(const EPuyoControl& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	int v;
	switch (in)
	{
	case EPuyoControl::PressDown:
		jsonEngineSettings["puyoEffectTime_PressDown"].get_to(v);
		break;
	case EPuyoControl::Control:
		jsonEngineSettings["puyoEffectTime_Control"].get_to(v);
		break;
	case EPuyoControl::Release:
		jsonEngineSettings["puyoEffectTime_Release"].get_to(v);
		break;
	case EPuyoControl::Vanish:
		jsonEngineSettings["puyoEffectTime_Vanish"].get_to(v);
		break;
	case EPuyoControl::VanishToFall:
		jsonEngineSettings["puyoEffectTime_VanishToFall"].get_to(v);
		break;
	default:
		jsonEngineSettings["puyoEffectTime_Wait"].get_to(v);
		break;
	}
	return v;
}
void GameSettings::SetPuyoEffectTime(const EPuyoControl& type, const int& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	auto set = [&jsonEngineSettings, &in](std::string id)
	{
		if (jsonEngineSettings.contains(id))
		{
			jsonEngineSettings[id] = in;
		}
	};
	switch (type)
	{
	case EPuyoControl::PressDown:
		set("puyoEffectTime_PressDown");
		break;
	case EPuyoControl::Control:
		set("puyoEffectTime_Control");
		break;
	case EPuyoControl::Release:
		set("puyoEffectTime_Release");
		break;
	case EPuyoControl::Vanish:
		set("puyoEffectTime_Vanish");
		break;
	case EPuyoControl::VanishToFall:
		set("puyoEffectTime_VanishToFall");
		break;
	default:
		set("puyoEffectTime_Wait");
		break;
	}
	GSSS::SaveJson(jsonEngineSettings);
}

int GameSettings::GetBonusCombo(int comboCount)
{
	json jsonEngineSettings = GSSS::GetJson();

	std::vector<int> v;
	jsonEngineSettings["comboBonus"].get_to(v);
	return v[comboCount];
}
void GameSettings::SetBonusCombo(const std::vector<int>& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	if (jsonEngineSettings.contains("comboBonus"))
	{
		jsonEngineSettings["comboBonus"] = in;
	}
	GSSS::SaveJson(jsonEngineSettings);
}

int GameSettings::GetBonusConnect(int connectCount)
{
	json jsonEngineSettings = GSSS::GetJson();

	std::vector<int> v;
	jsonEngineSettings["connectBonus"].get_to(v);
	return v[connectCount];
}
void GameSettings::SetBonusConnect(const std::vector<int>& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	if (jsonEngineSettings.contains("connectBonus"))
	{
		jsonEngineSettings["connectBonus"] = in;
	}
	GSSS::SaveJson(jsonEngineSettings);
}

int GameSettings::GetBonusColor(int colorCount)
{
	json jsonEngineSettings = GSSS::GetJson();

	std::vector<int> v;
	jsonEngineSettings["colorBonus"].get_to(v);
	return v[colorCount];
}
void GameSettings::SetBonusColor(const std::vector<int>& in)
{
	json jsonEngineSettings = GSSS::GetJson();

	if (jsonEngineSettings.contains("colorBonus"))
	{
		jsonEngineSettings["colorBonus"] = in;
	}
	GSSS::SaveJson(jsonEngineSettings);
}

std::wstring GameSettings::GetPuyoImage(int id)
{
	json jsonEngineSettings = GSSS::GetJson();

	std::vector<std::string> v;
	jsonEngineSettings["puyoImages"].get_to(v);

	return Util::s2WString(v[id]);
}

// -------------------------------------------------
// Settings : Game UI
// -------------------------------------------------
const FVector2D GameSettings::GAMEUI_SIZE = {
	300.f, 726.f
};
const FVector2D GameSettings::GAMEUI_LEFT_TOP = {
	GAMESCREEN_SIZE.x + GAMESCREEN_PADDING.x,
	GAMESCREEN_PADDING.y
};

const FVector2D GameSettings::GAMEUI_RIGHT_BOTTOM = {
	GAMESCREEN_SIZE.x + GAMEUI_SIZE.x,
	GAMESCREEN_SIZE.y + GAMEUI_SIZE.y - GAMESCREEN_PADDING.y
};

// -------------------------------------------------
// Settings : Game UI Next Puyo
// -------------------------------------------------
const FVector2D GameSettings::GAMEUI_NEXT_PUYO_LEFT_TOP = {
	GAMEUI_LEFT_TOP.x + GAMESCREEN_PADDING.x * 2,
	GAMEUI_LEFT_TOP.y + GAMESCREEN_PADDING.y
};