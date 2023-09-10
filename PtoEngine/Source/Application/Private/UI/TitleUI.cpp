
#include "UI/TitleUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/TextBlock.h"
#include "Slate/Button.h"
#include "Slate/Spacer.h"

#include "EngineSettings.h"
#include "GameSettings.h"

TitleUI::TitleUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(
		inOwner, 
		dx, 
		mouse,
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	const auto windowSize = EngineSettings::GetWindowSize();
	const auto padding = GameSettings::GAMESCREEN_PADDING;

	pRootSlate = std::make_shared<S_CanvasPanel>(windowSize, GetRt2D());
	pRootSlate->SetPosition({ 0, 0 });

	FSlateInfos menuVBInfos;
	menuVBInfos.VAlign = EVerticalAlignment::Center;
	const FVector2D menuVBSize = { windowSize.x / 2, windowSize.y / 2.f };
	auto pMenuVB = std::make_shared<S_VerticalBox>(menuVBSize, GetRt2D(), menuVBInfos);
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

		auto pTextBlock = std::make_shared<S_TextBlock>(GetRt2D(), infos, font, appearance);
		pTextBlock->SetText(L"pto8193 ‚Õ‚æ‚Õ‚æ");
		pMenuVB->AddChild(pTextBlock);
	}

	auto pSpacer = std::make_shared<S_Spacer>(FVector2D(0.f, 200.f), GetRt2D());
	pMenuVB->AddChild(pSpacer);

	/* TokoPuyo Button */
	auto button = [this, &pMenuVB](const std::wstring& mode)
	{
		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Center;
		infos.padding = { 0.f, 2.5f, 0.f, 2.5f };
		FSlateButtonAppearance appearance;
		auto pButton = std::make_shared<S_Button>(FVector2D(200.f, 40.f), GetRt2D(), infos, appearance);
		
		/* Label */
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Fill;
			//infos.VAlign = EVerticalAlignment::Fill;
			infos.padding = { 5.f };
			FSlateFont font;
			font.fontSize = 30.f;
			FSlateTextAppearance appearance;

			auto pTextBlock = std::make_shared<S_TextBlock>(GetRt2D(), infos, font, appearance);
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
	auto pButton_VSCPU = button(GameSettings::GetPuyoMode(1));
	pButton_VSCPU->OnClicked.Bind<&TitleUI::OnClickedVSCPU>(*this, "TitleUI");
	/* vs CPU */
	auto pButton_Exit = button(L"‚â‚ß‚é");
	pButton_Exit->OnClicked.Bind<&TitleUI::OnClickedExit>(*this, "TitleUI");

	pRootSlate->UpdateWidget();
}

TitleUI::TitleUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: TitleUI(nullptr, dx, mouse)
{
}

TitleUI::~TitleUI()
{
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

void TitleUI::OnClickedExit(DX::MouseEvent inMouseEvent)
{
	PostQuitMessage(0);
}