#pragma once

#include "UI/UserWidget.h"

class GameOverUI : public UserWidget
{
public:
	GameOverUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse, const int& MaxScore, const int& MaxCombo);
	GameOverUI(DirectX11& dx, DX::IMouseInterface* mouse, const int& MaxScore, const int& MaxCombo);
	virtual ~GameOverUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	void OnClickedRestartButton(DX::MouseEvent inMouseEvent);
	void OnClickedReturnTitle(DX::MouseEvent inMouseEvent);

	// ----------------------------------------------------------
	// Main : Delegates
	// ----------------------------------------------------------
	DX::FOnMouseAction OnClickedRestart;
	DX::FOnMouseAction OnClickedPause;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

};