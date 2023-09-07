#pragma once

#include "UI/UserWidget.h"

class S_TextBlock;
class S_Border;
class S_Image;
class S_Button;

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

	// ----------------------------------------------------------
	// Main : Delegates
	// ----------------------------------------------------------
	DX::FOnMouseAction OnClickedRestart;
	DX::FOnMouseAction OnClickedPause;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

	// ----------------------------------------------------------
	// State : Game State
	// ----------------------------------------------------------
	std::shared_ptr<S_TextBlock> pTextBlock_MaxScore = nullptr;
	std::shared_ptr<S_TextBlock> pTextBlock_MaxCombo = nullptr;

	std::shared_ptr<S_Button> pButton_Restart = nullptr;
};