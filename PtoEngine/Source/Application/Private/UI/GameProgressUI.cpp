
#include "UI/GameProgressUI.h"

#include "Slate/VerticalBox.h"
#include "Slate/CanvasPanel.h"
#include "Slate/Overlay.h"
#include "Slate/TextBlock.h"
#include "Slate/Border.h"
#include "Slate/Image.h"
#include "Slate/Button.h"

#include "GameSettings.h"
#include "EngineSettings.h"

using namespace DirectX;

GameProgressUI::GameProgressUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
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
	InfosVB->SetPosition(GameSettings::GAMEUI_LEFT_TOP);

	/* Next Puyo */
	{
		FSlateTextAppearance textAppearance;
		textAppearance.vAlign = EVerticalAlignment::Center;
		textAppearance.hAlign = EHorizontalAlignment::Center;
		FSlateInfos slateInfo;
		slateInfo.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pTextBlock_NextPuyo = std::make_shared<S_TextBlock>(GetRt2D(), slateInfo, FSlateFont(), textAppearance);
		pTextBlock_NextPuyo->SetText(L"Next Puyo");

		FSlateInfos Overlay;
		Overlay.padding = { 5.f, 15.f , 5.f, 15.f };
		Overlay.HAlign = EHorizontalAlignment::Center;
		auto pOverlay_NextPuyo1 = std::make_shared<S_Overlay>(FVector2D(150.f, 250.f), GetRt2D(), Overlay);

		FSlateInfos SlateInfoNextPuyo;
		SlateInfoNextPuyo.HAlign = EHorizontalAlignment::Center;
		SlateInfoNextPuyo.VAlign = EVerticalAlignment::Center;

		FSlateInfos SlateInfoNextPuyo1VB;
		SlateInfoNextPuyo1VB.HAlign = EHorizontalAlignment::Left;
		SlateInfoNextPuyo1VB.VAlign = EVerticalAlignment::Top;
		SlateInfoNextPuyo1VB.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pRootSlate_NextPuyo1 = std::make_shared<S_VerticalBox>(FVector2D(64.f, 128.f), GetRt2D(), SlateInfoNextPuyo1VB);

		if (pImage_NextPuyo1_1 == nullptr)
		{
			pImage_NextPuyo1_1 = std::make_shared<S_Image>(FVector2D(64.f, 64.f), GetRt2D(), SlateInfoNextPuyo);
		}
		if (pImage_NextPuyo1_2 == nullptr)
		{
			pImage_NextPuyo1_2 = std::make_shared<S_Image>(FVector2D(64.f, 64.f), GetRt2D(), SlateInfoNextPuyo);
		}

		FSlateInfos SlateInfoNextPuyo2VB;
		SlateInfoNextPuyo2VB.HAlign = EHorizontalAlignment::Right;
		SlateInfoNextPuyo2VB.VAlign = EVerticalAlignment::Bottom;
		SlateInfoNextPuyo2VB.padding = { 5.f, 5.f , 5.f, 5.f };
		auto pRootSlate_NextPuyo2 = std::make_shared<S_VerticalBox>(FVector2D(32.f, 64.f), GetRt2D(), SlateInfoNextPuyo2VB);
		if (pImage_NextPuyo2_1 == nullptr)
		{
			pImage_NextPuyo2_1 = std::make_shared<S_Image>(FVector2D(32.f, 32.f), GetRt2D(), SlateInfoNextPuyo);
		}
		if (pImage_NextPuyo2_2 == nullptr)
		{
			pImage_NextPuyo2_2 = std::make_shared<S_Image>(FVector2D(32.f, 32.f), GetRt2D(), SlateInfoNextPuyo);
		}
		InfosVB->AddChild(pTextBlock_NextPuyo);

		InfosVB->AddChild(pOverlay_NextPuyo1);

		pOverlay_NextPuyo1->AddChild(pRootSlate_NextPuyo1);
		pRootSlate_NextPuyo1->AddChild(pImage_NextPuyo1_1);
		pRootSlate_NextPuyo1->AddChild(pImage_NextPuyo1_2);

		pOverlay_NextPuyo1->AddChild(pRootSlate_NextPuyo2);
		pRootSlate_NextPuyo2->AddChild(pImage_NextPuyo2_1);
		pRootSlate_NextPuyo2->AddChild(pImage_NextPuyo2_2);
	}

	/* how to control */
	{
		FSlateInfos SlateInfos;
		SlateInfos.padding = { 5.f, 2.5f, 5.f, 2.5f };
		SlateInfos.HAlign = EHorizontalAlignment::Center;

		FSlateTextAppearance Appearance;
		Appearance.vAlign = EVerticalAlignment::Center;
		Appearance.hAlign = EHorizontalAlignment::Left;

		auto pText_Control1 = std::make_shared<S_TextBlock>(GetRt2D(), SlateInfos, FSlateFont(), Appearance);
		pText_Control1->SetText(L" ª : Turn Right");
		InfosVB->AddChild(pText_Control1);

		auto pText_Control2 = std::make_shared<S_TextBlock>(GetRt2D(), SlateInfos, FSlateFont(), Appearance);
		pText_Control2->SetText(L"© : Move Left");
		InfosVB->AddChild(pText_Control2);

		auto pText_Control3 = std::make_shared<S_TextBlock>(GetRt2D(), SlateInfos, FSlateFont(), Appearance);
		pText_Control3->SetText(L"¨ : Move Right");
		InfosVB->AddChild(pText_Control3);

		auto pText_Control4 = std::make_shared<S_TextBlock>(GetRt2D(), SlateInfos, FSlateFont(), Appearance);
		pText_Control4->SetText(L" « : Move Bottom");
		InfosVB->AddChild(pText_Control4);

		auto pText_Control5 = std::make_shared<S_TextBlock>(GetRt2D(), SlateInfos, FSlateFont(), Appearance);
		pText_Control5->SetText(L" Z : Turn Left");
		InfosVB->AddChild(pText_Control5);

		SlateInfos.padding = { 5.f, 2.5f, 5.f, 20.f };
		auto pText_Control6 = std::make_shared<S_TextBlock>(GetRt2D(), SlateInfos, FSlateFont(), Appearance);
		pText_Control6->SetText(L" X : Turn Right");
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
			pTextBlock_MaxScore = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (pTextBlock_MaxCombo == nullptr)
		{
			pTextBlock_MaxCombo = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (pTextBlock_Score == nullptr)
		{
			pTextBlock_Score = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), scoreAppearance);
		}
		if (pTextBlock_Combo == nullptr)
		{
			pTextBlock_Combo = std::make_shared<S_TextBlock>(GetRt2D(), scoreSlateInfos, FSlateFont(), scoreAppearance);
		}

		InfosVB->AddChild(pTextBlock_MaxScore);
		InfosVB->AddChild(pTextBlock_MaxCombo);
		InfosVB->AddChild(pTextBlock_Score);
		InfosVB->AddChild(pTextBlock_Combo);

		MaxScore = 0; MaxCombo = 0;
		UpdateScore(0, 0);
	}

	/* Restart Button */
	{
		if (pButton_Restart == nullptr)
		{
			FSlateInfos SlateInfos;
			SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
			pButton_Restart = std::make_shared<S_Button>(FVector2D(150.f, 40.f), pRt2D, SlateInfos);
			pButton_Restart->OnClicked.Bind<&GameProgressUI::OnClickedRestartButton>(*this, "Restart");
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

	/* Pause Button */
	{
		if (pButton_Pause == nullptr)
		{
			FSlateInfos SlateInfos;
			SlateInfos.padding = { 5.f, 5.f, 5.f, 5.f };
			pButton_Pause = std::make_shared<S_Button>(FVector2D(150.f, 40.f), pRt2D, SlateInfos);
			pButton_Pause->OnClicked.Bind<&GameProgressUI::OnClickedPauseButton>(*this, "Pause");
		}
		pButton_Pause->ClearChildren();
		InfosVB->AddChild(pButton_Pause);

		FSlateInfos LabelInfos;
		LabelInfos.VAlign = EVerticalAlignment::Center;
		LabelInfos.HAlign = EHorizontalAlignment::Center;
		auto pTextBlock_Pause = std::make_shared<S_TextBlock>(pRt2D, LabelInfos);
		pButton_Pause->AddChild(pTextBlock_Pause);
		pTextBlock_Pause->SetText(L"Pause");
	}

	pRootSlate->AddChild(InfosVB);
	pRootSlate->SetPosition({ 0, 0 });
	pRootSlate->UpdateWidget();
}
GameProgressUI::GameProgressUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: GameProgressUI(nullptr, dx, mouse)
{
}
GameProgressUI::~GameProgressUI()
{
	OnClickedRestart.ClearBind();
	OnClickedPause.ClearBind();

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

	pButton_Restart.reset();
	pButton_Restart = nullptr;
	pButton_Pause.reset();
	pButton_Pause = nullptr;

	pRootSlate->ClearChildren();
	pRootSlate.reset();
	pRootSlate = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
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
void GameProgressUI::UpdateScore(int inScore, int inCombo)
{
	MaxCombo = max(inCombo, MaxCombo);
	MaxScore = max(inScore, MaxScore);
	if (pTextBlock_MaxScore)
	{
		pTextBlock_MaxScore->SetText(std::format(L"Max Score : {}", MaxScore));
	}
	if (pTextBlock_MaxCombo)
	{
		pTextBlock_MaxCombo->SetText(std::format(L"Max Combo : {}", MaxCombo));
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
		static_cast<S_TextBlock*>(child)->SetText(L"Pause");
		bPause = false;
	}
	OnClickedRestart.Broadcast(inMouseEvent);
}
void GameProgressUI::OnClickedPauseButton(DX::MouseEvent inMouseEvent)
{
	SetIsPause(!bPause, inMouseEvent);
}

void GameProgressUI::SetIsPause(bool in, DX::MouseEvent inMouseEvent)
{
	SetIsPause(in);
	OnClickedPause.Broadcast(inMouseEvent);
}
void GameProgressUI::SetIsPause(bool in)
{
	bPause = in;
	if (bPause)
	{
		auto&& child = pButton_Pause->GetChildAt();
		static_cast<S_TextBlock*>(child)->SetText(L"Resume");
	}
	else
	{
		auto&& child = pButton_Pause->GetChildAt();
		static_cast<S_TextBlock*>(child)->SetText(L"Pause");
	}
}
bool GameProgressUI::IsPause() const noexcept
{
	return bPause;
}
