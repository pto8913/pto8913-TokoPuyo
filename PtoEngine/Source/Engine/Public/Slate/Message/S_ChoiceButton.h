#pragma once

#include "Slate/Button.h"

#include "Engine/Delegate.h"

#include "Algorithm/Array.h"

#include "ChoiceTypes.h"

class S_TextBlock;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChoice, DX::MouseEvent, const FChoiceInfos&);

class S_ChoiceButton : public S_Button
{
public:
	S_ChoiceButton(FVector2D inSize, ID2D1RenderTarget* inD2DRT, const FChoiceInfos& inChoiceInfos = {}, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	S_ChoiceButton(ID2D1RenderTarget* inD2DRT, const FChoiceInfos& inChoiceInfos = {}, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	virtual ~S_ChoiceButton();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	FOnChoice& GetOnChoice();

	// --------------------------
	// Main : Button
	// --------------------------
	void SetChoiceInfos(const FChoiceInfos& inChoiceInfos);

protected:
	void OnClickedEvent(DX::MouseEvent eventData);

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::shared_ptr<S_TextBlock> pLabel = nullptr;

	FOnChoice OnChoice;
	FChoiceInfos ChoiceInfos;
};