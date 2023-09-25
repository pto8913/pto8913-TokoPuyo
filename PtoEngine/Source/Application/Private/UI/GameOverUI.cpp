
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

#include "PtoGameInstance.h"

#include "World/WorldTypes.h"

using namespace DirectX;

GameOverUI::GameOverUI(
	Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse,
	const int& inMaxScore, const int& inMaxCombo
)
	: UserWidget(inOwner, inRt2D, dx, mouse),
	mMaxScore(inMaxScore),
	mMaxCombo(inMaxCombo)
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
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void GameOverUI::NativeOnInitialized()
{
	pRootSlate = MakeSlate<S_CanvasPanel>(EngineSettings::GetWindowSize());

	FSlateBorderAppearance colorMaskAppearance;
	colorMaskAppearance.color = FColor(0.f, 0.f, 0.f, 0.75f);
	auto colorMask = MakeSlate<S_Border>(EngineSettings::GetWindowSize(), FSlateInfos(), colorMaskAppearance);
	pRootSlate->AddChild(colorMask);

	FSlateInfos SlateInfos;
	SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
	auto InfosVB = MakeSlate<S_VerticalBox>(GameSettings::GAMEUI_SIZE, SlateInfos);
	InfosVB->SetPosition(
		{
			(EngineSettings::GetWindowSize().x / 2.f) - (GameSettings::GAMEUI_SIZE.x / 2.f),
			GameSettings::GAMESCREEN_PADDING.y
		}
	);

	auto pSpacer = MakeSlate<S_Spacer>(FVector2D(0.f, 80.f));
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
		auto pText_GameOver = MakeSlate<S_TextBlock>(*pDX, slateInfo, font, textGameOver);
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

		pTextBlock_MaxScore = MakeSlate<S_TextBlock>(*pDX, scoreSlateInfos, FSlateFont(), textAppearance);
		InfosVB->AddChild(pTextBlock_MaxScore);

		pTextBlock_MaxCombo = MakeSlate<S_TextBlock>(*pDX, scoreSlateInfos, FSlateFont(), textAppearance);
		InfosVB->AddChild(pTextBlock_MaxCombo);
		if (pTextBlock_MaxScore)
		{
			pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", mMaxScore));
		}
		if (pTextBlock_MaxCombo)
		{
			pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", mMaxCombo));
		}
	}
	InfosVB->AddChild(pSpacer);

	auto button = [this, &InfosVB](const std::wstring& label)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
		auto pButton = MakeSlate<S_Button>(FVector2D(150.f, 40.f), SlateInfos);

		FSlateInfos LabelInfos;
		LabelInfos.VAlign = EVerticalAlignment::Center;
		LabelInfos.HAlign = EHorizontalAlignment::Center;
		auto pLabel = MakeSlate<S_TextBlock>(*pDX, LabelInfos);
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
	UserWidget::NativeOnInitialized();
}
void GameOverUI::SetScore(const int& inMaxScore, const int& inMaxCombo)
{
	mMaxScore = inMaxScore;
	mMaxCombo = inMaxCombo;
	if (pTextBlock_MaxScore)
	{
		pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", mMaxScore));
	}
	if (pTextBlock_MaxCombo)
	{
		pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", mMaxCombo));
	}
}
void GameOverUI::OnClickedRestartButton(DX::MouseEvent inMouseEvent)
{
	OnClickedRestart.Broadcast(inMouseEvent);
}
void GameOverUI::OnClickedReturnTitle(DX::MouseEvent inMouseEvent)
{
	PtoGameInstance& gameInstance = PtoGameInstance::Get();
	gameInstance.OpenWorld(static_cast<int>(EWorldId::Title));
}
