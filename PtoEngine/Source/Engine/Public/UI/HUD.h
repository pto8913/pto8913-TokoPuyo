#pragma once

#include "UserWidget.h"
#include "HUDInterface.h"

#include "Engine/Delegate.h"
#include "Engine/Timer.h"

#include <chrono>

class S_ProgressBar;
class S_TextBlock;
class S_VerticalBox;
class S_GridPanel;
class S_Border;

class Level2D;

class HUD : public UserWidget
{
public:
	HUD(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH);
	HUD(DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH);

public:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------

	void AddSlate(std::shared_ptr<SlateBase> inSlate);
	void RemoveSlate(std::shared_ptr<SlateBase> inSlate);

	// --------------------------
	// Main : Status
	// --------------------------
	void OnHPChanged(int inCurrent, int inMax);

	// -----------------------------------------------------
	// Main : Game Infos
	// -----------------------------------------------------
	void NextFloor(const Level2D* pLevel);
	void PlayerMoved(const FVector& inPos);

	void SetFloorName(const std::wstring in);
	void ResetMap(const Level2D* pLevel);
	void UpdateMap(const Level2D* pLevel);

protected:
	// ------------------------------------------------------------------------------------------------------------
	// Settings
	// ------------------------------------------------------------------------------------------------------------

	// --------------------------
	// Settings : Game Infos : Map
	// --------------------------
	float mapSize = 6.f;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	std::shared_ptr<S_ProgressBar> pHPBar = nullptr;
	std::shared_ptr<S_TextBlock> pHPBarText = nullptr;

	// --------------------------
	// State : Game Message
	// --------------------------
	std::shared_ptr<S_VerticalBox> pGameMessage = nullptr;

	// --------------------------
	// State : Game Infos : Floor
	// --------------------------
	std::shared_ptr<S_TextBlock> pFloorText = nullptr;

	// --------------------------
	// State : Game Infos : Map
	// --------------------------
	std::shared_ptr<S_GridPanel> pMapGP = nullptr;
};