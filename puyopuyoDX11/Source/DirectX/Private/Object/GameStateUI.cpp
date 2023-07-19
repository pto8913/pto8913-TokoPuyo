
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

using namespace DirectX;

GameStateUI::GameStateUI(DirectX11& dx, UINT windowSizeW, UINT windowSizeH)
	: WidgetBase(dx, windowSizeW, windowSizeH)
{
	m_GameState = DX::GameState::Wait;
}

void GameStateUI::Draw()
{
	if (m_pRootSlate != nullptr)
	{
		switch (m_GameState)
		{
		case DX::GameState::GameOver:
			m_pRootSlate->Draw();
			break;
		default:
			m_pRootSlate->Draw();
			break;
		}
	}
}

// ----------------------------------------------------------
// Main : Game Progress
// ----------------------------------------------------------
void GameStateUI::SetGmaeProgressUI(DirectX11& dx)
{
	if (m_GameState == DX::GameState::Control)
	{
		return;
	}

	m_GameState = DX::GameState::Control;

	if (m_pRootSlate == nullptr)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		m_pRootSlate = std::make_shared<S_VerticalBox>(dx, Config::GAMEUI_SIZE, m_pRt2D, SlateInfos);
	}
	m_pImage_NextPuyo1_1.reset();
	m_pImage_NextPuyo1_2.reset();
	m_pImage_NextPuyo2_1.reset();
	m_pImage_NextPuyo2_2.reset();
	m_pTextBlock_MaxScore.reset();
	m_pTextBlock_MaxCombo.reset();
	m_pTextBlock_Score.reset();
	m_pTextBlock_Combo.reset();
	m_pButton_Restart.reset();
	m_pButton_Pause.reset();
	m_pRootSlate->ClearChildren();
	m_pRootSlate->SetPosition(Config::GAMEUI_LEFT_TOP);

	FSlateTextAppearance textAppearance;
	textAppearance.vAlign = EVerticalAlignment::Center;
	textAppearance.hAlign = EHorizontalAlignment::Center;
	FSlateInfos slateInfo;
	slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
	auto m_pTextBlock_NextPuyo = std::make_shared<S_TextBlock>(dx, m_pRt2D, slateInfo, FSlateFont(), textAppearance);
	m_pTextBlock_NextPuyo->SetText(L"Next Puyo");

	/* Next Puyo */
	FSlateInfos Overlay;
	Overlay.padding = { 5.f, 15.f , 5.f, 15.f };
	Overlay.HAlign = EHorizontalAlignment::Center;
	auto m_pOverlay_NextPuyo1 = std::make_shared<S_Overlay>(dx, XMFLOAT2(150.f, 250.f), m_pRt2D, Overlay);

	FSlateInfos SlateInfoNextPuyo;
	SlateInfoNextPuyo.HAlign = EHorizontalAlignment::Center;
	SlateInfoNextPuyo.VAlign = EVerticalAlignment::Center;

	FSlateInfos SlateInfoNextPuyo1VB;
	SlateInfoNextPuyo1VB.HAlign = EHorizontalAlignment::Left;
	SlateInfoNextPuyo1VB.VAlign = EVerticalAlignment::Top;
	SlateInfoNextPuyo1VB.padding = { 5.f, 5.f , 5.f, 5.f };
	auto m_pRootSlate_NextPuyo1 = std::make_shared<S_VerticalBox>(dx, XMFLOAT2(64.f, 128.f), m_pRt2D, SlateInfoNextPuyo1VB);

	if (m_pImage_NextPuyo1_1 == nullptr)
	{
		m_pImage_NextPuyo1_1 = std::make_shared<S_Image>(dx, XMFLOAT2(64.f, 64.f), m_pRt2D, SlateInfoNextPuyo);
	}
	if (m_pImage_NextPuyo1_2 == nullptr)
	{
		m_pImage_NextPuyo1_2 = std::make_shared<S_Image>(dx, XMFLOAT2(64.f, 64.f), m_pRt2D, SlateInfoNextPuyo);
	}

	FSlateInfos SlateInfoNextPuyo2VB;
	SlateInfoNextPuyo2VB.HAlign = EHorizontalAlignment::Right;
	SlateInfoNextPuyo2VB.VAlign = EVerticalAlignment::Bottom;
	SlateInfoNextPuyo2VB.padding = { 5.f, 5.f , 5.f, 5.f };
	auto m_pRootSlate_NextPuyo2 = std::make_shared<S_VerticalBox>(dx, XMFLOAT2(32.f, 64.f), m_pRt2D, SlateInfoNextPuyo2VB);
	if (m_pImage_NextPuyo2_1 == nullptr)
	{
		m_pImage_NextPuyo2_1 = std::make_shared<S_Image>(dx, XMFLOAT2(32.f, 32.f), m_pRt2D, SlateInfoNextPuyo);
	}
	if (m_pImage_NextPuyo2_2 == nullptr)
	{
		m_pImage_NextPuyo2_2 = std::make_shared<S_Image>(dx, XMFLOAT2(32.f, 32.f), m_pRt2D, SlateInfoNextPuyo);
	}
	m_pRootSlate->AddChild(m_pTextBlock_NextPuyo);

	m_pRootSlate->AddChild(m_pOverlay_NextPuyo1);

	m_pOverlay_NextPuyo1->AddChild(m_pRootSlate_NextPuyo1);
	m_pRootSlate_NextPuyo1->AddChild(m_pImage_NextPuyo1_1);
	m_pRootSlate_NextPuyo1->AddChild(m_pImage_NextPuyo1_2);

	m_pOverlay_NextPuyo1->AddChild(m_pRootSlate_NextPuyo2);
	m_pRootSlate_NextPuyo2->AddChild(m_pImage_NextPuyo2_1);
	m_pRootSlate_NextPuyo2->AddChild(m_pImage_NextPuyo2_2);

	/* how to control */
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		SlateInfos.HAlign = EHorizontalAlignment::Center;

		FSlateTextAppearance Appearance;
		Appearance.vAlign = EVerticalAlignment::Center;
		Appearance.hAlign = EHorizontalAlignment::Left;

		auto pText_Control1 = std::make_shared<S_TextBlock>(dx, m_pRt2D, SlateInfos, FSlateFont(), Appearance);
		pText_Control1->SetText(L" ª : Turn Right");
		m_pRootSlate->AddChild(pText_Control1);
		
		auto pText_Control2 = std::make_shared<S_TextBlock>(dx, m_pRt2D, SlateInfos, FSlateFont(), Appearance);
		pText_Control2->SetText(L"© : Move Left");
		m_pRootSlate->AddChild(pText_Control2);

		auto pText_Control3 = std::make_shared<S_TextBlock>(dx, m_pRt2D, SlateInfos, FSlateFont(), Appearance);
		pText_Control3->SetText(L"¨ : Move Right");
		m_pRootSlate->AddChild(pText_Control3);

		auto pText_Control4 = std::make_shared<S_TextBlock>(dx, m_pRt2D, SlateInfos, FSlateFont(), Appearance);
		pText_Control4->SetText(L" « : Move Bottom");
		m_pRootSlate->AddChild(pText_Control4);

		auto pText_Control5 = std::make_shared<S_TextBlock>(dx, m_pRt2D, SlateInfos, FSlateFont(), Appearance);
		pText_Control5->SetText(L"Z : Turn Left");
		m_pRootSlate->AddChild(pText_Control5);
		
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 20.f };
		auto pText_Control6 = std::make_shared<S_TextBlock>(dx, m_pRt2D, SlateInfos, FSlateFont(), Appearance);
		pText_Control6->SetText(L"X : Turn Right");
		m_pRootSlate->AddChild(pText_Control6);
	}

	/* Score */
	{
		FSlateInfos scoreSlateInfos;
		scoreSlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };

		FSlateTextAppearance scoreAppearance;
		scoreAppearance.vAlign = EVerticalAlignment::Center;
		scoreAppearance.hAlign = EHorizontalAlignment::Left;
		if (m_pTextBlock_MaxScore == nullptr)
		{
			m_pTextBlock_MaxScore = std::make_shared<S_TextBlock>(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (m_pTextBlock_MaxCombo == nullptr)
		{
			m_pTextBlock_MaxCombo = std::make_shared<S_TextBlock>(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (m_pTextBlock_Score == nullptr)
		{
			m_pTextBlock_Score = std::make_shared<S_TextBlock>(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (m_pTextBlock_Combo == nullptr)
		{
			m_pTextBlock_Combo = std::make_shared<S_TextBlock>(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
	}

	m_pRootSlate->AddChild(m_pTextBlock_MaxScore);
	m_pRootSlate->AddChild(m_pTextBlock_MaxCombo);
	m_pRootSlate->AddChild(m_pTextBlock_Score);
	m_pRootSlate->AddChild(m_pTextBlock_Combo);

	MaxScore = 0; MaxCombo = 0;
	UpdateScore(0, 0);

	CreateRestartButton(dx);
	CreatePauseButton(dx);
}
void GameStateUI::UpdateNextPuyo(UINT8 nPuyo1_1, UINT8 nPuyo1_2, UINT8 nPuyo2_1, UINT8 nPuyo2_2)
{
	if (m_pImage_NextPuyo1_1 != nullptr)
	{
		m_pImage_NextPuyo1_1->SetFileName(Config::PuyoImages[nPuyo1_2]);
	}

	if (m_pImage_NextPuyo1_2 != nullptr)
	{
		m_pImage_NextPuyo1_2->SetFileName(Config::PuyoImages[nPuyo1_1]);
	}

	if (m_pImage_NextPuyo2_1 != nullptr)
	{
		m_pImage_NextPuyo2_1->SetFileName(Config::PuyoImages[nPuyo2_2]);
	}

	if (m_pImage_NextPuyo2_2 != nullptr)
	{
		m_pImage_NextPuyo2_2->SetFileName(Config::PuyoImages[nPuyo2_1]);
	}
}
void GameStateUI::UpdateScore(int inScore, int inCombo)
{
	MaxCombo = max(inCombo, MaxCombo);
	MaxScore = max(inScore, MaxScore);
	if (m_pTextBlock_MaxScore)
	{
		m_pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", MaxScore));
	}
	if (m_pTextBlock_MaxCombo)
	{
		m_pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", MaxCombo));
	}
	if (m_pTextBlock_Score)
	{
		m_pTextBlock_Score->SetText(std::format(L"Score : {}", inScore));
	}
	if (m_pTextBlock_Combo)
	{
		m_pTextBlock_Combo->SetText(std::format(L"Combo : {}", inCombo));
	}
}

// ----------------------------------------------------------
// Main : Game Over
// ----------------------------------------------------------
void GameStateUI::SetGameOverUI(DirectX11& dx)
{
	if (m_GameState == DX::GameState::GameOver)
	{
		return;
	}

	m_GameState = DX::GameState::GameOver;

	if (m_pRootSlate == nullptr)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		m_pRootSlate = std::make_shared<S_VerticalBox>(dx, Config::GAMEUI_SIZE, m_pRt2D, SlateInfos);
	}
	m_pImage_NextPuyo1_1.reset();
	m_pImage_NextPuyo1_2.reset();
	m_pImage_NextPuyo2_1.reset();
	m_pImage_NextPuyo2_2.reset();
	m_pTextBlock_MaxScore.reset();
	m_pTextBlock_MaxCombo.reset();
	m_pTextBlock_Score.reset();
	m_pTextBlock_Combo.reset();
	m_pButton_Restart.reset();
	m_pButton_Pause.reset();

	m_pRootSlate->ClearChildren();
	m_pRootSlate->SetPosition(
		{
			(Config::windowSize.x / 2.f) - (Config::GAMEUI_SIZE.x / 2.f),
			Config::GAMESCREEN_PADDING.y
		}
	);

	auto m_pSpacer = std::make_shared<S_Spacer>(dx, XMFLOAT2(0.f, 80.f), m_pRt2D);
	m_pRootSlate->AddChild(m_pSpacer);

	FSlateTextAppearance textGameOver;
	textGameOver.vAlign = EVerticalAlignment::Center;
	textGameOver.hAlign = EHorizontalAlignment::Center;
	FSlateInfos slateInfo;
	slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
	auto pText_GameOver = std::make_shared<S_TextBlock>(dx, m_pRt2D, slateInfo, FSlateFont(), textGameOver);
	pText_GameOver->SetText(L"GAME OVER");
	m_pRootSlate->AddChild(pText_GameOver);

	FSlateInfos scoreSlateInfos;
	scoreSlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };

	FSlateTextAppearance textAppearance;
	textAppearance.vAlign = EVerticalAlignment::Center;
	textAppearance.hAlign = EHorizontalAlignment::Left;

	if (m_pTextBlock_MaxScore == nullptr)
	{
		m_pTextBlock_MaxScore = std::make_shared<S_TextBlock>(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), textAppearance);
	}
	m_pRootSlate->AddChild(m_pTextBlock_MaxScore);

	if (m_pTextBlock_MaxCombo == nullptr)
	{
		m_pTextBlock_MaxCombo = std::make_shared<S_TextBlock>(dx, m_pRt2D, scoreSlateInfos, FSlateFont(), textAppearance);
	}
	m_pRootSlate->AddChild(m_pTextBlock_MaxCombo);
	UpdateScore(0, 0);

	m_pRootSlate->AddChild(m_pSpacer);

	CreateRestartButton(dx);
}

void GameStateUI::CreateRestartButton(DirectX11& dx)
{
	if (m_pButton_Restart == nullptr)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
		m_pButton_Restart = std::make_shared<S_Button>(dx, XMFLOAT2(150.f, 40.f), m_pRt2D, SlateInfos);
		m_pButton_Restart->OnClicked.Bind<&GameStateUI::OnClickedRestartButton>(*this);
	}
	m_pButton_Restart->ClearChildren();
	m_pRootSlate->AddChild(m_pButton_Restart);

	FSlateInfos LabelInfos;
	LabelInfos.VAlign = EVerticalAlignment::Center;
	LabelInfos.HAlign = EHorizontalAlignment::Center;
	auto m_pTextBlock_Restart = std::make_shared<S_TextBlock>(dx, m_pRt2D, LabelInfos);
	m_pButton_Restart->AddChild(m_pTextBlock_Restart);
	m_pTextBlock_Restart->SetText(L"Restart");
}
void GameStateUI::CreatePauseButton(DirectX11& dx)
{
	if (m_pButton_Pause == nullptr)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
		m_pButton_Pause = std::make_shared<S_Button>(dx, XMFLOAT2(150.f, 40.f), m_pRt2D, SlateInfos);
		m_pButton_Pause->OnClicked.Bind<&GameStateUI::OnClickedPauseButton>(*this);
	}
	m_pButton_Pause->ClearChildren();
	m_pRootSlate->AddChild(m_pButton_Pause);

	FSlateInfos LabelInfos;
	LabelInfos.VAlign = EVerticalAlignment::Center;
	LabelInfos.HAlign = EHorizontalAlignment::Center;
	auto m_pTextBlock_Pause = std::make_shared<S_TextBlock>(dx, m_pRt2D, LabelInfos);
	m_pButton_Pause->AddChild(m_pTextBlock_Pause);
	m_pTextBlock_Pause->SetText(L"Pause");
}
void GameStateUI::OnClickedRestartButton(DX::MouseEvent inMouseEvent)
{
	OnClickedRestart.Broadcast(inMouseEvent);
}
void GameStateUI::OnClickedPauseButton(DX::MouseEvent inMouseEvent)
{
	bPause = !bPause;
	if (bPause)
	{
		auto&& child = m_pButton_Pause->GetSlotAt();
		static_cast<S_TextBlock*>(child)->SetText(L"Resume");
	}
	else
	{
		auto&& child = m_pButton_Pause->GetSlotAt();
		static_cast<S_TextBlock*>(child)->SetText(L"Pause");
	}
	OnClickedPause.Broadcast(inMouseEvent);

}
bool GameStateUI::IsPause() const noexcept
{
	return bPause;
}
