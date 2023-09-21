
#include "Slate/Message/S_ChoiceButton.h"

#include "Slate/VerticalBox.h"
#include "Slate/Button.h"
#include "Slate/TextBlock.h"

S_ChoiceButton::S_ChoiceButton(ID2D1RenderTarget* inRt2D, DirectX11& dx, FVector2D inSize, const FChoiceInfos& inChoiceInfos, FSlateInfos inSlateInfos, FSlateButtonAppearance inButtonAppearance)
	: S_Button(inRt2D, inSize, inSlateInfos, inButtonAppearance)
{
	pDX = &dx;
	OnClicked.Bind<&S_ChoiceButton::OnClickedEvent>(*this, "S_ChoiceButton");

	FSlateInfos infos;
	infos.HAlign = EHorizontalAlignment::Center;
	infos.VAlign = EVerticalAlignment::Center;
	pLabel = std::make_shared<S_TextBlock>(inRt2D, infos);
	AddChild(pLabel);

	SetChoiceInfos(inChoiceInfos);
}
S_ChoiceButton::S_ChoiceButton(ID2D1RenderTarget* inRt2D, DirectX11& dx, const FChoiceInfos& inChoiceInfos, FSlateInfos inSlateInfos, FSlateButtonAppearance inButtonAppearance)
	: S_ChoiceButton(inRt2D, dx, { 0,0 }, inChoiceInfos, inSlateInfos, inButtonAppearance)
{
}
S_ChoiceButton::~S_ChoiceButton()
{
	ClearChildren();
	pLabel.reset();
	pLabel = nullptr;

	OnChoice.ClearBind();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
FOnChoice& S_ChoiceButton::GetOnChoice()
{
	return OnChoice;
}

// --------------------------
// Main : Button
// --------------------------
void S_ChoiceButton::SetChoiceInfos(const FChoiceInfos& inChoiceInfos)
{
	if (ChoiceInfos == inChoiceInfos)
	{
		return;
	}
	ChoiceInfos = inChoiceInfos;

	pLabel->SetText(ChoiceInfos.buttonLabel);
}

void S_ChoiceButton::OnClickedEvent(DX::MouseEvent eventData)
{
	OnChoice.Broadcast(eventData, ChoiceInfos);
}