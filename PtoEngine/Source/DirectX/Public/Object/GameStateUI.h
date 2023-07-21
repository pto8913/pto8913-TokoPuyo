#pragma once

#include "DrawableObject2D.h"
#include "GameUIController.h"
#include "Render/Factory/TransformConstantBuffer.h"

#include "Widget/WidgetBase.h"

class TransformConstantBuffer;

class DirectX11;

class S_TextBlock;
class S_Image;
class S_Button;
class S_VerticalBox;
class S_Overlay;
class S_Spacer;

class ScreenTextOnlyOutput;

class GameStateUI : public WidgetBase
{
public:
	GameStateUI(DirectX11& dx, UINT windowSizeW, UINT windowSizeH);

private:
	virtual void Draw() override;
public:
	// ----------------------------------------------------------
	// Main : Game Progress
	// ----------------------------------------------------------
	virtual void SetGmaeProgressUI();

	void UpdateNextPuyo(UINT8 nPuyo1_1, UINT8 nPuyo1_2, UINT8 nPuyo2_1, UINT8 nPuyo2_2);
	void UpdateScore(int inScore, int inCombo);

	// ----------------------------------------------------------
	// Main : Game Over
	// ----------------------------------------------------------
	virtual void SetGameOverUI();

	void CreateRestartButton();
	void CreatePauseButton();
	void OnClickedRestartButton(DX::MouseEvent inMouseEvent);
	void OnClickedPauseButton(DX::MouseEvent inMouseEvent);

	bool IsPause() const noexcept;

	// ----------------------------------------------------------
	// Main : Event
	// ----------------------------------------------------------
	DX::FOnMouseAction OnClickedRestart;
	DX::FOnMouseAction OnClickedPause;
private:
	// --------------------------------------------------------------------------------------------------------------------
	// State
	// --------------------------------------------------------------------------------------------------------------------
	DX::GameState m_GameState;
	bool bPause = false;

	// ----------------------------------------------------------
	// State : Game State
	// ----------------------------------------------------------
	std::shared_ptr<S_Image> m_pImage_NextPuyo1_1 = nullptr;
	std::shared_ptr<S_Image> m_pImage_NextPuyo1_2 = nullptr;

	std::shared_ptr<S_Image> m_pImage_NextPuyo2_1 = nullptr;
	std::shared_ptr<S_Image> m_pImage_NextPuyo2_2 = nullptr;

	std::shared_ptr<S_TextBlock> m_pTextBlock_MaxScore = nullptr;
	std::shared_ptr<S_TextBlock> m_pTextBlock_MaxCombo = nullptr;

	std::shared_ptr<S_TextBlock> m_pTextBlock_Score = nullptr;
	std::shared_ptr<S_TextBlock> m_pTextBlock_Combo = nullptr;

	std::shared_ptr<S_Button> m_pButton_Restart = nullptr;
	std::shared_ptr<S_Button> m_pButton_Pause = nullptr;

	int MaxScore = 0;
	int MaxCombo = 0;
};