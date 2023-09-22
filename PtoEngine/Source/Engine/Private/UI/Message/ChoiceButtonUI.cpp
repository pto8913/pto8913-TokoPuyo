
#include "UI/Message/ChoiceButtonUI.h"

#include "Slate/Button.h"
#include "Slate/TextBlock.h"

#include "EngineSettings.h"

ChoiceButtonUI::ChoiceButtonUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse, const FChoiceInfos& inChoiceInfos)
	: UserWidget(inOwner, inRt2D, dx, mouse)
{
	SetChoiceInfos(inChoiceInfos);
}
ChoiceButtonUI::~ChoiceButtonUI()
{
	pLabel.reset();
	pLabel = nullptr;

	OnChoice.ClearBind();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void ChoiceButtonUI::NativeOnInitialized()
{
	/* Buttob */
	{
		FSlateInfos infos;
		infos.padding.top = 2.5f;
		infos.padding.bottom = 2.5f;
		FSlateButtonAppearance appearance;
		appearance.HoverColor = FColor(0.f, 1.f, 0.75f, 1.f);
		appearance.PressColor = FColor(0.f, 0.5f, 0.4f, 1.f);
		auto button = MakeSlate<S_Button>(FVector2D(400.f, 50.f), infos);;
		button->OnClicked.Bind<&ChoiceButtonUI::OnClickedEvent>(*this, "ChoiceButtonUI");

		pRootSlate = std::move(button);
		pRootSlate->SetPosition({ 0, 0 });
	}

	/* Label */
	{
		FSlateInfos textInfos;
		textInfos.padding = { 1.f };
		FSlateFont font;
		FSlateTextAppearance appearance;
		appearance.color = FColor(255.f, 0.f, 0.f, 1.f);
		appearance.hAlign = EHorizontalAlignment::Center;
		appearance.vAlign = EVerticalAlignment::Center;
		pLabel = MakeSlate<S_TextBlock>(*pDX, FVector2D(500.f, 30.f), textInfos, font, appearance);
		pRootSlate->AddChild(pLabel);
	}

	pRootSlate->UpdateWidget();
	UserWidget::NativeOnInitialized();
}
FOnChoice& ChoiceButtonUI::GetOnChoice()
{
	return OnChoice;
}

// --------------------------
// Main : Button
// --------------------------
void ChoiceButtonUI::SetChoiceInfos(const FChoiceInfos& inChoiceInfos)
{
	if (mChoiceInfos == inChoiceInfos)
	{
		return;
	}
	mChoiceInfos = inChoiceInfos;

	pLabel->SetText(mChoiceInfos.buttonLabel);
}

void ChoiceButtonUI::OnClickedEvent(DX::MouseEvent eventData)
{
	OnChoice.Broadcast(eventData, mChoiceInfos);
}