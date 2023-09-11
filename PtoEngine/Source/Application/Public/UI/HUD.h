#pragma once

#include "UI/UserWidget.h"
#include "HUDInterface.h"
#include "Input/MouseInterface.h"

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
	HUD(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse);
	HUD(DirectX11& dx, DX::IMouseInterface* mouse);

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void Draw() override;

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
	void SetMap(const Level2D* pLevel, std::shared_ptr<S_Border> pCell, const int& x, const int& y);

public:
	// -----------------------------------------------------
	// Main : Debug
	// -----------------------------------------------------
	void AddBoxDebug(std::shared_ptr<class BoxCollision2D> in);
	void DrawDebugScreen();

protected:
#if _DEBUG
	// --------------------------
	// State : Debug
	// --------------------------
	std::vector<std::shared_ptr<class BoxCollision2D>> pBoxDebug;
	std::vector<std::shared_ptr<S_Border>> pScreenGrid;
#endif

	// ------------------------------------------------------------------------------------------------------------
	// Settings
	// ------------------------------------------------------------------------------------------------------------

	// --------------------------
	// Settings : Game Infos : Map
	// --------------------------
	float mapSize = 6.f;

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