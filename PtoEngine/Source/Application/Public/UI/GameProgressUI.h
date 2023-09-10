#pragma once

#include "UI/UserWidget.h"

class S_TextBlock;
class S_Image;
class S_Button;

class GameProgressUI : public UserWidget
{
public:
	GameProgressUI(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse);
	GameProgressUI(DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~GameProgressUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	void UpdateNextPuyo(uint8_t nPuyo1_1, uint8_t nPuyo1_2, uint8_t nPuyo2_1, uint8_t nPuyo2_2);
	void UpdateScore(const int& inScore, const int& inCombo, const int& inMaxScore, const int& inMaxCombo);

protected:
	void OnClickedRestartButton(DX::MouseEvent inMouseEvent);
	void OnClickedPauseButton(DX::MouseEvent inMouseEvent);
	void OnClickedReturnToTitle(DX::MouseEvent inMouseEvent);
public:

	// ----------------------------------------------------------
	// Main : Delegates
	// ----------------------------------------------------------
	DX::FOnMouseAction OnClickedRestart;
	DX::FOnMouseAction OnClickedPause;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	
	// ----------------------------------------------------------
	// State : Game State
	// ----------------------------------------------------------
	std::shared_ptr<S_Image> pImage_NextPuyo1_1 = nullptr;
	std::shared_ptr<S_Image> pImage_NextPuyo1_2 = nullptr;

	std::shared_ptr<S_Image> pImage_NextPuyo2_1 = nullptr;
	std::shared_ptr<S_Image> pImage_NextPuyo2_2 = nullptr;

	std::shared_ptr<S_TextBlock> pTextBlock_MaxScore = nullptr;
	std::shared_ptr<S_TextBlock> pTextBlock_MaxCombo = nullptr;

	std::shared_ptr<S_TextBlock> pTextBlock_Score = nullptr;
	std::shared_ptr<S_TextBlock> pTextBlock_Combo = nullptr;

	std::shared_ptr<S_Button> pButton_Pause = nullptr;

	bool bPause = false;
};