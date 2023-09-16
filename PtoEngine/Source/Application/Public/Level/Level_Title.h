#pragma once

#include "Framework/Level/Level.h"
#include "Input/MouseInterface.h"

class TitleUI;
class SettingsUI;
class DebugUI;

class Level_Title : public Level
{
public:
	Level_Title(DirectX11& dx);
	virtual ~Level_Title();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Level_Title" + "#"s + std::to_string(mID);
	}
	virtual void BeginPlay(DirectX11& dx) override;

	// --------------------------
	// Main : Play TokoPuyo
	// --------------------------
	void OnClickedTokoPuyo(DX::MouseEvent inMouseEvent);

	// --------------------------
	// Main : SettingsUI
	// --------------------------
	void OnClickedOpenSettings(DX::MouseEvent inMouseEvent);
protected:
	void OnClickedCloseSettings(DX::MouseEvent inMouseEvent);

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	TitleUI* pTitleUI = nullptr;
	SettingsUI* pSettingsUI = nullptr;
	DebugUI* pDebugUI = nullptr;
};