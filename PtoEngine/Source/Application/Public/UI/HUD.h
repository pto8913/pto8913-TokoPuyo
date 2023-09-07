#pragma once

#include "UI/UserWidget.h"
#include "HUDInterface.h"

#include "Engine/Delegate.h"
#include "Engine/Timer.h"

#include <chrono>

class S_TextBlock;
class S_VerticalBox;
class S_GridPanel;
class S_Border;

class Level2D;

class HUD : public UserWidget
{
public:
	HUD(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse);
	HUD(DirectX11& dx, DX::IMouseInterface* mouse);

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void Draw() override;

	// -----------------------------------------------------
	// Main : Game Infos
	// -----------------------------------------------------
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

	// --------------------------
	// State : Game Message
	// --------------------------
	std::shared_ptr<S_VerticalBox> pGameMessage = nullptr;

};