
#include "Object/GameStateUI.h"

#include "Core/DirectX.h"

#include <dwrite.h>

#include "Config.h"

#include "Slate/Button.h"
#include "Slate/TextBlock.h"
#include "Slate/Image.h"
#include "Slate/VerticalBox.h"
#include "Slate/Overlay.h"
#include "Slate/Spacer.h"

#include <format>

GameStateUI::GameStateUI(DirectX11& dx)
	: WidgetBase(dx)
{
	m_GameState = DX::GameState::Control;
}

void GameStateUI::ExecuteTasks2(DirectX11& dx)
{
	ExecuteTasks(dx, dx.GetContext());
}
void GameStateUI::Draw()
{
	switch (m_GameState)
	{
	case DX::GameState::GameOver:
		break;
	default:
		m_pVerticalBox->Draw();
		break;
	}
}

// ----------------------------------------------------------
// Main : Game Progress
// ----------------------------------------------------------
void GameStateUI::SetGmaeProgressUI(DirectX11& dx)
{
	m_GameState = DX::GameState::Control;

	if (m_pVerticalBox == nullptr)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		m_pVerticalBox = new S_VerticalBox(dx, Config::GAMEUI_SIZE, m_pRt2D, SlateInfos);
		m_pVerticalBox->SetPosition(Config::GAMEUI_LEFT_TOP);
	}
	FSlateTextAppearance textAppearance;
	textAppearance.vAlign = EVerticalAlignment::Center;
	if (m_pTextBlock_NextPuyo == nullptr)
	{
		textAppearance.hAlign = EHorizontalAlignment::Center;
		FSlateInfos slateInfo;
		slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
		m_pTextBlock_NextPuyo = new S_TextBlock(dx, m_pRt2D, slateInfo, FSlateFont(), textAppearance);
		m_pVerticalBox->AddChild(m_pTextBlock_NextPuyo);
		m_pTextBlock_NextPuyo->SetText(L"Next Puyo");
	}
	textAppearance.hAlign = EHorizontalAlignment::Left;

	FSlateInfos SlateInfoNextPuyo;
	SlateInfoNextPuyo.HAlign = EHorizontalAlignment::Center;
	SlateInfoNextPuyo.VAlign = EVerticalAlignment::Center;

	FSlateInfos SlateInfoNextPuyoVB;
	SlateInfoNextPuyoVB.HAlign = EHorizontalAlignment::Center;
	SlateInfoNextPuyoVB.VAlign = EVerticalAlignment::Center;
	SlateInfoNextPuyoVB.padding = { 5.f, 5.f , 5.f, 5.f };
	if (m_pVerticalBox_NextPuyo1 == nullptr)
	{
		FSlateInfos Overlay;
		Overlay.padding = { 5.f, 5.f , 5.f, 5.f };
		m_pOverlay_NextPuyo1 = new S_Overlay(dx, {0.f, 150.f}, m_pRt2D, Overlay);
		m_pVerticalBox->AddChild(m_pOverlay_NextPuyo1);

		//m_pSpacer_NextPuyo1 = new S_Spacer(dx, { 0.f, 150.f }, m_pRt2D);
		//m_pOverlay_NextPuyo1->AddChild(m_pSpacer_NextPuyo1);

		m_pVerticalBox_NextPuyo1 = new S_VerticalBox(dx, { 64.f, 128.f }, m_pRt2D, SlateInfoNextPuyoVB);
		m_pOverlay_NextPuyo1->AddChild(m_pVerticalBox_NextPuyo1);

		m_pImage_NextPuyo1_1 = new S_Image(dx, { 64.f, 64.f }, m_pRt2D, SlateInfoNextPuyo);
		m_pVerticalBox_NextPuyo1->AddChild(m_pImage_NextPuyo1_1);

		m_pImage_NextPuyo1_2 = new S_Image(dx, { 64.f, 64.f }, m_pRt2D, SlateInfoNextPuyo);
		m_pVerticalBox_NextPuyo1->AddChild(m_pImage_NextPuyo1_2);
	}

	if (m_pVerticalBox_NextPuyo2 == nullptr)
	{
		FSlateInfos Overlay;
		Overlay.padding = { 5.f, 5.f , 5.f, 5.f };
		m_pOverlay_NextPuyo2 = new S_Overlay(dx, { 0.f, 150.f }, m_pRt2D, Overlay);
		m_pVerticalBox->AddChild(m_pOverlay_NextPuyo2);

		//m_pSpacer_NextPuyo2 = new S_Spacer(dx, { 0.f, 150.f }, m_pRt2D);
		//m_pOverlay_NextPuyo2->AddChild(m_pSpacer_NextPuyo2);

		m_pVerticalBox_NextPuyo2 = new S_VerticalBox(dx, { 32.f, 64.f }, m_pRt2D, SlateInfoNextPuyoVB);
		m_pOverlay_NextPuyo2->AddChild(m_pVerticalBox_NextPuyo2);

		m_pImage_NextPuyo2_1 = new S_Image(dx, { 32.f, 32.f }, m_pRt2D, SlateInfoNextPuyo);
		m_pVerticalBox_NextPuyo2->AddChild(m_pImage_NextPuyo2_1);

		m_pImage_NextPuyo2_2 = new S_Image(dx, { 32.f, 32.f }, m_pRt2D, SlateInfoNextPuyo);
		m_pVerticalBox_NextPuyo2->AddChild(m_pImage_NextPuyo2_2);
	}

	if (m_pTextBlock_MaxScore == nullptr)
	{
		FSlateInfos scoreSlateInfos;
		scoreSlateInfos.margin.top = 5.f;
		scoreSlateInfos.margin.bottom = 5.f;

		m_pTextBlock_MaxScore = new S_TextBlock(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), textAppearance);
		m_pVerticalBox->AddChild(m_pTextBlock_MaxScore);

		m_pTextBlock_MaxCombo = new S_TextBlock(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), textAppearance);
		m_pVerticalBox->AddChild(m_pTextBlock_MaxCombo);

		m_pTextBlock_Score = new S_TextBlock(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), textAppearance);
		m_pVerticalBox->AddChild(m_pTextBlock_Score);

		m_pTextBlock_Combo = new S_TextBlock(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), textAppearance);
		m_pVerticalBox->AddChild(m_pTextBlock_Combo);
	}
	if (m_pButton_Restart == nullptr)
	{
		FSlateInfos SlateInfos;
		SlateInfos.margin = { 5.f, 5.f, 5.f, 5.f };
		m_pButton_Restart = new S_Button(dx, m_pRt2D, SlateInfos);
		m_pVerticalBox->AddChild(m_pButton_Restart);
	}
}
void GameStateUI::UpdateNextPuyo(DirectX11& dx, UINT8 nPuyo1_1, UINT8 nPuyo1_2, UINT8 nPuyo2_1, UINT8 nPuyo2_2)
{
	m_pImage_NextPuyo1_1->SetFileName(Config::PuyoImages[nPuyo1_2]);
	m_pImage_NextPuyo1_2->SetFileName(Config::PuyoImages[nPuyo1_1]);
	m_pImage_NextPuyo2_1->SetFileName(Config::PuyoImages[nPuyo2_2]);
	m_pImage_NextPuyo2_2->SetFileName(Config::PuyoImages[nPuyo2_1]);

	ExecuteTasks2(dx);
}
void GameStateUI::UpdateScore(DirectX11& dx, int inScore, int inCombo)
{
	MaxCombo = max(inCombo, MaxCombo);
	MaxScore = max(inScore, MaxScore);
	m_pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", MaxScore));
	m_pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", MaxCombo));
	m_pTextBlock_Score->SetText(std::format(L"Score : {}", inScore));
	m_pTextBlock_Combo->SetText(std::format(L"Combo : {}", inCombo));
	
	ExecuteTasks2(dx);
}

// ----------------------------------------------------------
// Main : Game Over
// ----------------------------------------------------------
void GameStateUI::SetGameOverUI(DirectX11& dx)
{
	m_GameState = DX::GameState::GameOver;

}

