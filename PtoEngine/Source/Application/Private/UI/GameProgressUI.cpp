
#include "UI/GameProgressUI.h"

#include "Slate/VerticalBox.h"
#include "Slate/CanvasPanel.h"
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

GameProgressUI::GameProgressUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(inOwner, inRt2D, dx, mouse)
{
}
GameProgressUI::~GameProgressUI()
{
	OnClickedRestart.ClearBind();
	OnClickedPause.ClearBind();

	pOverlay_NextPuyo.reset();
	pOverlay_NextPuyo = nullptr;

	pImage_NextPuyo1_1.reset();
	pImage_NextPuyo1_1 = nullptr;
	pImage_NextPuyo1_2.reset();
	pImage_NextPuyo1_2 = nullptr;

	pImage_NextPuyo2_1.reset();
	pImage_NextPuyo2_1 = nullptr;
	pImage_NextPuyo2_2.reset();
	pImage_NextPuyo2_2 = nullptr;

	pTextBlock_MaxScore.reset();
	pTextBlock_MaxScore = nullptr;
	pTextBlock_MaxCombo.reset();
	pTextBlock_MaxCombo = nullptr;

	pTextBlock_Score.reset();
	pTextBlock_Score = nullptr;
	pTextBlock_Combo.reset();
	pTextBlock_Combo = nullptr;

	pButton_Pause.reset();
	pButton_Pause = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void GameProgressUI::NativeOnInitialized()
{
	pRootSlate = MakeSlate<S_CanvasPanel>(EngineSettings::GetWindowSize());

	FSlateInfos SlateInfos;
	SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
	auto InfosVB = MakeSlate<S_VerticalBox>(FVector2D(300.f, 768.f), SlateInfos);
	pRootSlate->AddChild(InfosVB);
	InfosVB->SetPosition({ 416, GameSettings::GAMESCREEN_PADDING.y });

	/* GameMode Title */
	{
		FSlateTextAppearance textAppearance;
		textAppearance.vAlign = EVerticalAlignment::Center;
		textAppearance.hAlign = EHorizontalAlignment::Center;
		FSlateFont font;
		font.fontSize = 30.f;
		FSlateInfos slateInfo;
		slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pTextBlock_GameMode = MakeSlate<S_TextBlock>(slateInfo, font, textAppearance);
		pTextBlock_GameMode->SetText(L"TokoPuyo");

		InfosVB->AddChild(pTextBlock_GameMode);
	}

	/* Next Puyo */
	{
		FSlateTextAppearance textAppearance;
		textAppearance.vAlign = EVerticalAlignment::Center;
		textAppearance.hAlign = EHorizontalAlignment::Center;
		FSlateInfos slateInfo;
		slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pTextBlock_NextPuyo = MakeSlate<S_TextBlock>(slateInfo, FSlateFont(), textAppearance);
		pTextBlock_NextPuyo->SetText(L"Next Puyo");
		//InfosVB->AddChild(pTextBlock_NextPuyo);

		FSlateInfos Overlay;
		Overlay.padding = { 5.f, 15.f , 5.f, 15.f };
		Overlay.HAlign = EHorizontalAlignment::Center;
		pOverlay_NextPuyo = MakeSlate<S_Overlay>(FVector2D(150.f, 250.f), Overlay);
		InfosVB->AddChild(pOverlay_NextPuyo);

		FSlateInfos SlateInfoNextPuyo;
		SlateInfoNextPuyo.HAlign = EHorizontalAlignment::Center;
		SlateInfoNextPuyo.VAlign = EVerticalAlignment::Center;
		{
			FSlateInfos SlateInfoNextPuyo1VB;
			SlateInfoNextPuyo1VB.HAlign = EHorizontalAlignment::Left;
			SlateInfoNextPuyo1VB.VAlign = EVerticalAlignment::Top;
			SlateInfoNextPuyo1VB.padding = { 5.f, 5.f , 5.f, 5.f };
			auto pRootSlate_NextPuyo1 = MakeSlate<S_VerticalBox>(FVector2D(64.f, 128.f), SlateInfoNextPuyo1VB);

			if (pImage_NextPuyo1_1 == nullptr)
			{
				pImage_NextPuyo1_1 = MakeSlate<S_Image>(FVector2D(64.f, 64.f), SlateInfoNextPuyo);
			}
			if (pImage_NextPuyo1_2 == nullptr)
			{
				pImage_NextPuyo1_2 = MakeSlate<S_Image>(FVector2D(64.f, 64.f), SlateInfoNextPuyo);
			}
			pOverlay_NextPuyo->AddChild(pRootSlate_NextPuyo1);
			pRootSlate_NextPuyo1->AddChild(pImage_NextPuyo1_1);
			pRootSlate_NextPuyo1->AddChild(pImage_NextPuyo1_2);
		}
		{
			FSlateInfos SlateInfoNextPuyo2VB;
			SlateInfoNextPuyo2VB.HAlign = EHorizontalAlignment::Right;
			SlateInfoNextPuyo2VB.VAlign = EVerticalAlignment::Bottom;
			SlateInfoNextPuyo2VB.padding = { 5.f, 5.f , 5.f, 5.f };
			auto pRootSlate_NextPuyo2 = MakeSlate<S_VerticalBox>(FVector2D(32.f, 64.f), SlateInfoNextPuyo2VB);
			if (pImage_NextPuyo2_1 == nullptr)
			{
				pImage_NextPuyo2_1 = MakeSlate<S_Image>(FVector2D(32.f, 32.f), SlateInfoNextPuyo);
			}
			if (pImage_NextPuyo2_2 == nullptr)
			{
				pImage_NextPuyo2_2 = MakeSlate<S_Image>(FVector2D(32.f, 32.f), SlateInfoNextPuyo);
			}
			pOverlay_NextPuyo->AddChild(pRootSlate_NextPuyo2);
			pRootSlate_NextPuyo2->AddChild(pImage_NextPuyo2_1);
			pRootSlate_NextPuyo2->AddChild(pImage_NextPuyo2_2);
		}
	}

	/* how to control */
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		//SlateInfos.HAlign = EHorizontalAlignment::Center;

		FSlateTextAppearance Appearance;
		Appearance.vAlign = EVerticalAlignment::Center;
		Appearance.hAlign = EHorizontalAlignment::Left;

		auto pText_Control1 = MakeSlate<S_TextBlock>(SlateInfos, FSlateFont(), Appearance);
		pText_Control1->SetText(L" ↑ : Turn Right");

		auto pText_Control2 = MakeSlate<S_TextBlock>(SlateInfos, FSlateFont(), Appearance);
		pText_Control2->SetText(L"← : Move Left");

		auto pText_Control3 = MakeSlate<S_TextBlock>(SlateInfos, FSlateFont(), Appearance);
		pText_Control3->SetText(L"→ : Move Right");

		auto pText_Control4 = MakeSlate<S_TextBlock>(SlateInfos, FSlateFont(), Appearance);
		pText_Control4->SetText(L" ↓ : Move Bottom");

		auto pText_Control5 = MakeSlate<S_TextBlock>(SlateInfos, FSlateFont(), Appearance);
		pText_Control5->SetText(L" Z : Turn Left");

		SlateInfos.padding = { 5.f, 2.5f, 5.f, 20.f };
		auto pText_Control6 = MakeSlate<S_TextBlock>(SlateInfos, FSlateFont(), Appearance);
		pText_Control6->SetText(L" X : Turn Right");

		InfosVB->AddChild(pText_Control1);
		InfosVB->AddChild(pText_Control2);
		InfosVB->AddChild(pText_Control3);
		InfosVB->AddChild(pText_Control4);
		InfosVB->AddChild(pText_Control5);
		InfosVB->AddChild(pText_Control6);
	}

	/* Score */
	{
		FSlateInfos scoreSlateInfos;
		scoreSlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };

		FSlateTextAppearance scoreAppearance;
		scoreAppearance.vAlign = EVerticalAlignment::Center;
		scoreAppearance.hAlign = EHorizontalAlignment::Left;
		if (pTextBlock_MaxScore == nullptr)
		{
			pTextBlock_MaxScore = MakeSlate<S_TextBlock>(scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (pTextBlock_MaxCombo == nullptr)
		{
			pTextBlock_MaxCombo = MakeSlate<S_TextBlock>(scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (pTextBlock_Score == nullptr)
		{
			pTextBlock_Score = MakeSlate<S_TextBlock>(scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (pTextBlock_Combo == nullptr)
		{
			pTextBlock_Combo = MakeSlate<S_TextBlock>(scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		InfosVB->AddChild(pTextBlock_MaxScore);
		InfosVB->AddChild(pTextBlock_MaxCombo);
		InfosVB->AddChild(pTextBlock_Score);
		InfosVB->AddChild(pTextBlock_Combo);

		UpdateScore(0, 0, 0, 0);
	}

	auto button = [this, &InfosVB](const std::wstring& label)
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		auto pButton = MakeSlate<S_Button>(FVector2D(150.f, 40.f), SlateInfos);
		pButton->ClearChildren();

		FSlateInfos LabelInfos;
		LabelInfos.VAlign = EVerticalAlignment::Center;
		LabelInfos.HAlign = EHorizontalAlignment::Center;
		auto pLabel = MakeSlate<S_TextBlock>(LabelInfos);
		pButton->AddChild(pLabel);
		pLabel->SetText(label);

		InfosVB->AddChild(pButton);
		return std::move(pButton);
	};
	/* Restart Button */
	auto pButton_Restart = button(L"リスタート");
	pButton_Restart->OnClicked.Bind<&GameProgressUI::OnClickedRestartButton>(*this, "Restart");

	/* Pause Button */
	pButton_Pause = button(L"中断");
	pButton_Pause->OnClicked.Bind<&GameProgressUI::OnClickedPauseButton>(*this, "Pause");

	/* Title Button */
	auto pButton_Title = button(L"タイトルに戻る");
	pButton_Title->OnClicked.Bind<&GameProgressUI::OnClickedReturnToTitle>(*this, "Title");

	pRootSlate->SetPosition({ 0, 0 });
	pRootSlate->UpdateWidget();
	UserWidget::NativeOnInitialized();
}
void GameProgressUI::UpdateNextPuyo(uint8_t nPuyo1_1, uint8_t nPuyo1_2, uint8_t nPuyo2_1, uint8_t nPuyo2_2)
{
	if (pImage_NextPuyo1_1 != nullptr)
	{
		pImage_NextPuyo1_1->SetFileName(GameSettings::GetPuyoImage(nPuyo1_2));
	}

	if (pImage_NextPuyo1_2 != nullptr)
	{
		pImage_NextPuyo1_2->SetFileName(GameSettings::GetPuyoImage(nPuyo1_1));
	}

	if (pImage_NextPuyo2_1 != nullptr)
	{
		pImage_NextPuyo2_1->SetFileName(GameSettings::GetPuyoImage(nPuyo2_2));
	}

	if (pImage_NextPuyo2_2 != nullptr)
	{
		pImage_NextPuyo2_2->SetFileName(GameSettings::GetPuyoImage(nPuyo2_1));
	}
}
void GameProgressUI::UpdateScore(const int& inScore, const int& inCombo, const int& inMaxScore, const int& inMaxCombo)
{
	if (pTextBlock_MaxScore)
	{
		pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", inMaxScore));
	}
	if (pTextBlock_MaxCombo)
	{
		pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", inMaxCombo));
	}
	if (pTextBlock_Score)
	{
		pTextBlock_Score->SetText(std::format(L"Score : {}", inScore));
	}
	if (pTextBlock_Combo)
	{
		pTextBlock_Combo->SetText(std::format(L"Combo : {}", inCombo));
	}
}

void GameProgressUI::OnClickedRestartButton(DX::MouseEvent inMouseEvent)
{
	if (bPause)
	{
		auto&& child = pButton_Pause->GetChildAt();
		static_cast<S_TextBlock*>(child)->SetText(L"中断");
		bPause = false;
	}
	OnClickedRestart.Broadcast(inMouseEvent);
}
void GameProgressUI::OnClickedPauseButton(DX::MouseEvent inMouseEvent)
{
	bPause = !bPause;
	if (bPause)
	{
		auto&& child = pButton_Pause->GetChildAt();
		static_cast<S_TextBlock*>(child)->SetText(L"再開");
	}
	else
	{
		auto&& child = pButton_Pause->GetChildAt();
		static_cast<S_TextBlock*>(child)->SetText(L"中断");
	}
	OnClickedPause.Broadcast(inMouseEvent);
}
void GameProgressUI::OnClickedReturnToTitle(DX::MouseEvent inMouseEvent)
{
	PtoGameInstance& gameInstance = PtoGameInstance::Get();
	gameInstance.OpenWorld(static_cast<int>(EWorldId::Title));
}
