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

class HUD : public UserWidget, public DX::HUDInterface
{
public:
	HUD(DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH);
	HUD(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH);

private:
	virtual void Draw() override final;

public:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------

	// --------------------------
	// Main : Status
	// --------------------------
	void OnHPChanged(int inCurrent, int inMax);

	// -----------------------------------------------------
	// Main : HUDInterface
	// -----------------------------------------------------
	virtual ID2D1RenderTarget* GetRt2D() override;

	// --------------------------
	// Main : HUDInterface : External
	// --------------------------
	virtual void AddSlate(std::shared_ptr<SlateBase> inSlate) override;
	virtual void RemoveSlate(std::shared_ptr<SlateBase> inSlate) override;

	// --------------------------
	// Main : HUDInterface : Game Infos : Map
	// --------------------------
	virtual void ResetMap(const Level2D* pLevel);
	virtual void UpdateMap(const Level2D* pLevel) override;

	// --------------------------
	// Main : HUDInterface : Effect
	// --------------------------
	virtual void Effect_Blackout(float playRate = 0.5f) override;
	void Effect_Blckout_Completed();
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
	std::shared_ptr<S_ProgressBar> m_pHPBar = nullptr;
	std::shared_ptr<S_TextBlock> m_pHPText = nullptr;

	// --------------------------
	// State : Game Message
	// --------------------------
	std::shared_ptr<S_VerticalBox> m_pGameMessage = nullptr;

	// --------------------------
	// State : Game Infos : Floor
	// --------------------------
	std::shared_ptr<S_TextBlock> m_pFloorText = nullptr;

	// --------------------------
	// State : Game Infos : Map
	// --------------------------
	std::shared_ptr<S_GridPanel> m_pMapGP = nullptr;

	// --------------------------
	// State : HUDInterface : Effect
	// --------------------------
	std::shared_ptr<S_Border> pEffectBorder = nullptr;
};