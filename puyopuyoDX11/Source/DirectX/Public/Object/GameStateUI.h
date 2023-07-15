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

class GameStateUI : public WidgetBase, public DX::IGameStateUIControl
{
public:
	GameStateUI(DirectX11& dx);
	virtual void ExecuteTasks2(DirectX11& dx) override;

private:
	virtual void Draw() override;
public:
	virtual void SetPause() override {};

	// ----------------------------------------------------------
	// Main : Game Progress
	// ----------------------------------------------------------
	virtual void SetGmaeProgressUI(DirectX11& dx) override;

	void UpdateNextPuyo(DirectX11& dx, UINT8 nPuyo1_1, UINT8 nPuyo1_2, UINT8 nPuyo2_1, UINT8 nPuyo2_2);
	void UpdateScore(DirectX11& dx, int inScore, int inCombo);

	// ----------------------------------------------------------
	// Main : Game Over
	// ----------------------------------------------------------
	virtual void SetGameOverUI(DirectX11& dx) override;

private:
	// --------------------------------------------------------------------------------------------------------------------
	// State
	// --------------------------------------------------------------------------------------------------------------------

	DX::GameState m_GameState;
	// ----------------------------------------------------------
	// State : Game State
	// ----------------------------------------------------------
	S_VerticalBox* m_pVerticalBox = nullptr;
	
	S_TextBlock* m_pTextBlock_NextPuyo = nullptr;

	S_Overlay* m_pOverlay_NextPuyo1 = nullptr;
	S_Spacer* m_pSpacer_NextPuyo1 = nullptr;
	S_VerticalBox* m_pVerticalBox_NextPuyo1 = nullptr;
	S_Image* m_pImage_NextPuyo1_1 = nullptr;
	S_Image* m_pImage_NextPuyo1_2 = nullptr;

	S_Overlay* m_pOverlay_NextPuyo2 = nullptr;
	S_Spacer* m_pSpacer_NextPuyo2 = nullptr;
	S_VerticalBox* m_pVerticalBox_NextPuyo2 = nullptr;
	S_Image* m_pImage_NextPuyo2_1 = nullptr;
	S_Image* m_pImage_NextPuyo2_2 = nullptr;

	S_TextBlock* m_pTextBlock_MaxScore = nullptr;
	S_TextBlock* m_pTextBlock_MaxCombo = nullptr;

	S_TextBlock* m_pTextBlock_Score = nullptr;
	S_TextBlock* m_pTextBlock_Combo = nullptr;

	S_Button* m_pButton_Restart = nullptr;

	int MaxScore = 0;
	int MaxCombo = 0;
};