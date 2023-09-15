#pragma once

#include "UI/UserWidget.h"

class GameOverUI : public UserWidget
{
public:
	GameOverUI(
		Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse, 
		const int& MaxScore, const int& MaxCombo
	);
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