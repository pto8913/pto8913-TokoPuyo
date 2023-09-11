
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

#include "PuyoGameInstance.h"

using namespace DirectX;

GameOverUI::GameOverUI(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse, const int& MaxScore, const int& MaxCombo)
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
		textGameOver.color = FColor(255.f, 0.f, 0.f, 1.f);
		FSlateFont font;
		font.fontSize = 40.f;
		FSlateInfos slateInfo;
		slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pText_GameOver = std::make_shared<S_TextBlock>(GetRt2D(), slateInfo, font, textGameOver);
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

		auto pTextBlock_MaxScore = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), textAppearance);
		InfosVB->AddChild(pTextBlock_MaxScore);

		auto pTextBlock_MaxCombo = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), textAppearance);
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

	auto button = [this, &InfosVB](const std::wstring& label)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
		auto pButton = std::make_shared<S_Button>(FVector2D(150.f, 40.f), pRt2D, SlateInfos);

		FSlateInfos LabelInfos;
		LabelInfos.VAlign = EVerticalAlignment::Center;
		LabelInfos.HAlign = EHorizontalAlignment::Center;
		auto pLabel = std::make_shared<S_TextBlock>(pRt2D, LabelInfos);
		pLabel->SetText(label);
		pButton->AddChild(pLabel);

		InfosVB->AddChild(pButton);
		return std::move(pButton);
	};
	/* Restart Button */
	auto pButton_Restart = button(L"リスタート");
	pButton_Restart->OnClicked.Bind<&GameOverUI::OnClickedRestartButton>(*this, "GameOverUI");
	/* Title Button */
	auto pButton_Title = button(L"タイトルに戻る");
	pButton_Title->OnClicked.Bind<&GameOverUI::OnClickedReturnTitle>(*this, "GameOverUI");

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

	if (pRootSlate)
	{
		pRootSlate->ClearChildren();
	}
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
void GameOverUI::OnClickedReturnTitle(DX::MouseEvent inMouseEvent)
{
	PuyoGameInstance& gameInstance = PuyoGameInstance::Get();
	gameInstance.OpenWorld(GetWorld(), EWorldId::Title);
}
