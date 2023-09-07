
#include "UI/GameOverUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/Overlay.h"
#include "Slate/TextBlock.h"
#include "Slate/Border.h"
#include "Slate/Image.h"
#include "Slate/Button.h"
#include "Slate/Spacer.h"

#include "GameSettings.h"
#include "EngineSettings.h"

using namespace DirectX;

GameOverUI::GameOverUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse, const int& MaxScore, const int& MaxCombo)
	: UserWidget(
		inOwner,
		dx,
		mouse,
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	pRootSlate = std::make_shared<S_CanvasPanel>(EngineSettings::GetWindowSize(), GetRt2D());

	FSlateInfos SlateInfos;
	SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
	auto InfosVB = std::make_shared<S_VerticalBox>(GameSettings::GAMEUI_SIZE, GetRt2D(), SlateInfos);
	InfosVB->SetPosition(
		{
			(EngineSettings::GetWindowSize().x / 2.f) - (GameSettings::GAMEUI_SIZE.x / 2.f),
			GameSettings::GAMESCREEN_PADDING.y
		}
	);

	auto pSpacer = std::make_shared<S_Spacer>(FVector2D(0.f, 80.f), GetRt2D());
	InfosVB->AddChild(pSpacer);

	/* GameOverText */
	{
		FSlateTextAppearance textGameOver;
		textGameOver.vAlign = EVerticalAlignment::Center;
		textGameOver.hAlign = EHorizontalAlignment::Center;
		FSlateInfos slateInfo;
		slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pText_GameOver = std::make_shared<S_TextBlock>(GetRt2D(), slateInfo, FSlateFont(), textGameOver);
		pText_GameOver->SetText(L"GAME OVER");
		InfosVB->AddChild(pText_GameOver);
	}

	/* Result : Score */
	{
		FSlateInfos scoreSlateInfos;
		scoreSlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };

		FSlateTextAppearance textAppearance;
		textAppearance.vAlign = EVerticalAlignment::Center;
		textAppearance.hAlign = EHorizontalAlignment::Left;

		if (pTextBlock_MaxScore == nullptr)
		{
			pTextBlock_MaxScore = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), textAppearance);
		}
		InfosVB->AddChild(pTextBlock_MaxScore);

		if (pTextBlock_MaxCombo == nullptr)
		{
			pTextBlock_MaxCombo = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), textAppearance);
		}
		InfosVB->AddChild(pTextBlock_MaxCombo);
		if (pTextBlock_MaxScore)
		{
			pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", MaxScore));
		}
		if (pTextBlock_MaxCombo)
		{
			pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", MaxCombo));
		}
	}
	InfosVB->AddChild(pSpacer);

	/* Restart Button */
	{
		if (pButton_Restart == nullptr)
		{
			FSlateInfos SlateInfos;
			SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
			pButton_Restart = std::make_shared<S_Button>(FVector2D(150.f, 40.f), pRt2D, SlateInfos);
			pButton_Restart->OnClicked.Bind<&GameOverUI::OnClickedRestartButton>(*this, "Restart");
		}
		pButton_Restart->ClearChildren();
		InfosVB->AddChild(pButton_Restart);

		FSlateInfos LabelInfos;
		LabelInfos.VAlign = EVerticalAlignment::Center;
		LabelInfos.HAlign = EHorizontalAlignment::Center;
		auto pTextBlock_Restart = std::make_shared<S_TextBlock>(pRt2D, LabelInfos);
		pButton_Restart->AddChild(pTextBlock_Restart);
		pTextBlock_Restart->SetText(L"Restart");
	}

	pRootSlate->AddChild(InfosVB);
	pRootSlate->SetPosition({ 0, 0 });
	pRootSlate->UpdateWidget();
}
GameOverUI::GameOverUI(DirectX11& dx, DX::IMouseInterface* mouse, const int& MaxScore, const int& MaxCombo)
	: GameOverUI(nullptr, dx, mouse, MaxScore, MaxCombo)
{
}
GameOverUI::~GameOverUI()
{
	OnClickedRestart.ClearBind();
	OnClickedPause.ClearBind();

	pTextBlock_MaxScore.reset();
	pTextBlock_MaxScore = nullptr;
	pTextBlock_MaxCombo.reset();
	pTextBlock_MaxCombo = nullptr;

	pButton_Restart.reset();
	pButton_Restart = nullptr;

	pRootSlate->ClearChildren();
	pRootSlate.reset();
	pRootSlate = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void GameOverUI::OnClickedRestartButton(DX::MouseEvent inMouseEvent)
{
	OnClickedRestart.Broadcast(inMouseEvent);
}
