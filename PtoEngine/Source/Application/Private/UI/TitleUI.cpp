
#include "UI/TitleUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/TextBlock.h"
#include "Slate/Button.h"
#include "Slate/Spacer.h"

#include "EngineSettings.h"
#include "GameSettings.h"

TitleUI::TitleUI(Object* inOwner, int inID, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(inOwner, inID, inRt2D, dx, mouse)
{
	const auto windowSize = EngineSettings::GetWindowSize();
	const auto padding = GameSettings::GAMESCREEN_PADDING;

	pRootSlate = std::make_shared<S_CanvasPanel>(windowSize, inRt2D);
	pRootSlate->SetPosition({ 0, 0 });

	FSlateInfos menuVBInfos;
	menuVBInfos.VAlign = EVerticalAlignment::Center;
	const FVector2D menuVBSize = { windowSize.x / 2, windowSize.y / 2.f };
	auto pMenuVB = std::make_shared<S_VerticalBox>(menuVBSize, inRt2D, menuVBInfos);
	pMenuVB->SetPosition({ windowSize.x / 2 - menuVBSize.x / 2, windowSize.y / 2 - menuVBSize.y / 2 + padding.y });
	pRootSlate->AddChild(pMenuVB);

	/* Title */
	{
		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Fill;
		infos.VAlign = EVerticalAlignment::Fill;
		FSlateFont font;
		font.fontSize = 60.f;
		FSlateTextAppearance appearance;

		auto pTextBlock = std::make_shared<S_TextBlock>(inRt2D, infos, font, appearance);
		pTextBlock->SetText(L"pto8193 ぷよぷよ");
		pMenuVB->AddChild(pTextBlock);
	}

	auto pSpacer = std::make_shared<S_Spacer>(FVector2D(0.f, 100.f), inRt2D);
	pMenuVB->AddChild(pSpacer);

	/* Button */
	auto button = [this, &pMenuVB, &inRt2D](const std::wstring& mode)
	{
		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Center;
		infos.padding = { 0.f, 2.5f, 0.f, 2.5f };
		FSlateButtonAppearance appearance;
		auto pButton = std::make_shared<S_Button>(FVector2D(200.f, 40.f), inRt2D, infos, appearance);
		
		/* Label */
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Fill;
			//infos.VAlign = EVerticalAlignment::Fill;
			infos.padding = { 5.f };
			FSlateFont font;
			font.fontSize = 30.f;
			FSlateTextAppearance appearance;

			auto pTextBlock = std::make_shared<S_TextBlock>(inRt2D, infos, font, appearance);
			pTextBlock->SetText(mode);
			pButton->AddChild(pTextBlock);
		}
		pMenuVB->AddChild(pButton);
		return std::move(pButton);
	};
	/* TokoPuyo */
	auto pButton_TokoPuyo = button(GameSettings::GetPuyoMode(0));
	pButton_TokoPuyo->OnClicked.Bind<&TitleUI::OnClickedTokoPuyoButton>(*this, "TitleUI");
	/* vs CPU */
	//auto pButton_VSCPU = button(GameSettings::GetPuyoMode(1));
	//pButton_VSCPU->OnClicked.Bind<&TitleUI::OnClickedVSCPU>(*this, "TitleUI");
	/* Settings */
	auto pButton_Settings = button(L"設定");
	pButton_Settings->OnClicked.Bind<&TitleUI::OnClickedSettings>(*this, "TitleUI");
	/* Exit */
	auto pButton_Exit = button(L"やめる");
	pButton_Exit->OnClicked.Bind<&TitleUI::OnClickedExit>(*this, "TitleUI");

	pRootSlate->UpdateWidget();
}
TitleUI::~TitleUI()
{
	OnClickedTokoPuyo.ClearBind();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void TitleUI::OnClickedTokoPuyoButton(DX::MouseEvent inMouseEvent)
{
	OnClickedTokoPuyo.Broadcast(inMouseEvent);
}

void TitleUI::OnClickedVSCPU(DX::MouseEvent inMouseEvent)
{
	OnClickedTokoPuyo.Broadcast(inMouseEvent);
}

void TitleUI::OnClickedSettings(DX::MouseEvent inMouseEvent)
{
	OnClickedOpenSettings.Broadcast(inMouseEvent);
}

void TitleUI::OnClickedExit(DX::MouseEvent inMouseEvent)
{
	PostQuitMessage(0);
}