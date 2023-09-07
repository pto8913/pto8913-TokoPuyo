
#include "EngineSettings.h"

#include "nlohmann/json.hpp"
#include <fstream>

using namespace nlohmann;

EngineSettings::EngineSettings(const FVector2D& inSize, const std::wstring& inTitle)
{
	SetWindowSize(inSize);
	SetWindowTitle(inTitle);
}

namespace ESSS
{
	json GetJson()
	{
		std::ifstream loadEngineSettings("Config/PtoEngineSettings.json");
		json jsonEngineSettings;
		loadEngineSettings >> jsonEngineSettings;
		return jsonEngineSettings;
	}
	void SaveJson(json inJson)
	{
		std::ofstream saveEngineSettings("Config/PtoEngineSettings.json");
		saveEngineSettings << std::setw(4) << inJson << std::endl;
	}
}

// -------------------------------------------------
// Settings : Application
// -------------------------------------------------
void EngineSettings::SetWindowSize(const FVector2D& inWindowSize)
{
	json jsonEngineSettings = ESSS::GetJson();

	if (jsonEngineSettings.contains("WindowSize"))
	{
		jsonEngineSettings["WindowSize"] = { inWindowSize.x, inWindowSize.y };
	}
	ESSS::SaveJson(jsonEngineSettings);
}
FVector2D EngineSettings::GetWindowSize()
{
	json jsonEngineSettings = ESSS::GetJson();

	std::vector<float> v;
	jsonEngineSettings["windowSize"].get_to(v);
	return FVector2D(v[0], v[1]);
}

void EngineSettings::SetWindowTitle(const std::wstring& inWindowTitle)
{
	json jsonEngineSettings = ESSS::GetJson();

	if (jsonEngineSettings.contains("windowTitle"))
	{
		jsonEngineSettings["windowTitle"] = inWindowTitle;
	}
	ESSS::SaveJson(jsonEngineSettings);
}
std::wstring EngineSettings::GetWindowTitle()
{
	json jsonEngineSettings = ESSS::GetJson();
	return jsonEngineSettings["windowAspectRatio"].get<std::wstring>();
}

void EngineSettings::SetWindowAspectRatio(const FVector2D& inWindowAspectRatio)
{
	json jsonEngineSettings = ESSS::GetJson();

	if (jsonEngineSettings.contains("windowAspectRatio"))
	{
		jsonEngineSettings["windowAspectRatio"] = { inWindowAspectRatio.x, inWindowAspectRatio.y };
	}
	ESSS::SaveJson(jsonEngineSettings);
}
FVector2D EngineSettings::GetWindowAspectRatio()
{
	json jsonEngineSettings = ESSS::GetJson();

	std::vector<float> v;
	jsonEngineSettings["windowAspectRatio"].get_to(v);
	return FVector2D(v[0], v[1]);
}

// -----------------------
// Settings : Engine 2D
// -----------------------
void EngineSettings::SetGameScreen2DCellSize(const FVector2D& inGameScreen2DCellSize)
{
	json jsonEngineSettings = ESSS::GetJson();

	if (jsonEngineSettings.contains("game2DCellSize"))
	{
		jsonEngineSettings["game2DCellSize"] = { inGameScreen2DCellSize.x, inGameScreen2DCellSize.y };
	}
	ESSS::SaveJson(jsonEngineSettings);
}
FVector2D EngineSettings::GetGameScreen2DCellSize()
{
	json jsonEngineSettings = ESSS::GetJson();

	std::vector<float> v;
	jsonEngineSettings["game2DCellSize"].get_to(v);
	return FVector2D(v[0], v[1]);
}
FVector2D EngineSettings::GetCELL()
{
	return GetWindowAspectRatio() * GetGameScreen2DCellSize();
}
FVector2D EngineSettings::GETCELL(const FVector2D& inSize)
{
	return GetWindowAspectRatio() * inSize;
}

void EngineSettings::SetGameScreen2DSize(const FVector2D& inGameScreen2DSize)
{
	json jsonEngineSettings = ESSS::GetJson();

	if (jsonEngineSettings.contains("gameScreen2DCellSize"))
	{
		jsonEngineSettings["gameScreen2DCellSize"] = { inGameScreen2DSize.x, inGameScreen2DSize.y };
	}
	ESSS::SaveJson(jsonEngineSettings);

	SetGameScreen2DCenter({ (float)(int)(inGameScreen2DSize.x / 2) - 1, (float)(int)(inGameScreen2DSize.y / 2) - 1 });
}
FVector2D EngineSettings::GetGameScreen2DSize()
{
	json jsonEngineSettings = ESSS::GetJson();
	std::vector<float> v;
	jsonEngineSettings["gameScreen2DCellSize"].get_to(v);
	return FVector2D(v[0], v[1]);
}

void EngineSettings::SetGameScreen2DCenter(const FVector2D& inGameScreen2DCenter)
{
	json jsonEngineSettings = ESSS::GetJson();

	if (jsonEngineSettings.contains("gameScreen2DCenter"))
	{
		jsonEngineSettings["gameScreen2DCenter"] = { inGameScreen2DCenter.x, inGameScreen2DCenter.y };
	}
	ESSS::SaveJson(jsonEngineSettings);
}
FVector2D EngineSettings::GetGameScreen2DCenter()
{
	json jsonEngineSettings = ESSS::GetJson();

	std::vector<int> v;
	jsonEngineSettings["gameScreen2DCenter"].get_to(v);
	return FVector2D(v[0], v[1]);
}
