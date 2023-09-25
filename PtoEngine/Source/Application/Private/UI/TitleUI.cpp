
#include "UI/TitleUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/TextBlock.h"
#include "Slate/Button.h"
#include "Slate/Spacer.h"

#include "EngineSettings.h"
#include "GameSettings.h"

TitleUI::TitleUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(inOwner, inRt2D, dx, mouse)
{
}
TitleUI::~TitleUI()
{
	OnClickedTokoPuyo.ClearBind();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void TitleUI::NativeOnInitialized()
{
	const auto windowSize = EngineSettings::GetWindowSize();
	const auto padding = GameSettings::GAMESCREEN_PADDING;

	pRootSlate = MakeSlate<S_CanvasPanel>(windowSize);
	pRootSlate->SetPosition({ 0, 0 });

	FSlateInfos menuVBInfos;
	menuVBInfos.VAlign = EVerticalAlignment::Center;
	const FVector2D menuVBSize = { windowSize.x / 2, windowSize.y / 2.f };
	auto pMenuVB = MakeSlate<S_VerticalBox>(menuVBSize, menuVBInfos);
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

		auto pTextBlock = MakeSlate<S_TextBlock>(*pDX, infos, font, appearance);
		pTextBlock->SetText(L"pto8913 ‚Õ‚æ‚Õ‚æ");
		pMenuVB->AddChild(pTextBlock);
	}

	auto pSpacer = MakeSlate<S_Spacer>(FVector2D(0.f, 100.f));
	pMenuVB->AddChild(pSpacer);

	/* Button */
	auto button = [this, &pMenuVB](const std::wstring& mode)
	{
		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Center;
		infos.padding = { 0.f, 2.5f, 0.f, 2.5f };
		FSlateButtonAppearance appearance;
		auto pButton = MakeSlate<S_Button>(FVector2D(200.f, 40.f), infos, appearance);

		/* Label */
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Fill;
			//infos.VAlign = EVerticalAlignment::Fill;
			infos.padding = { 5.f };
			FSlateFont font;
			font.fontSize = 30.f;
			FSlateTextAppearance appearance;

			auto pTextBlock = MakeSlate<S_TextBlock>(*pDX, infos, font, appearance);
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
	auto pButton_Settings = button(L"Ý’è");
	pButton_Settings->OnClicked.Bind<&TitleUI::OnClickedSettings>(*this, "TitleUI");
	/* Exit */
	auto pButton_Exit = button(L"‚â‚ß‚é");
	pButton_Exit->OnClicked.Bind<&TitleUI::OnClickedExit>(*this, "TitleUI");

	pRootSlate->UpdateWidget();
	UserWidget::NativeOnInitialized();
}
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