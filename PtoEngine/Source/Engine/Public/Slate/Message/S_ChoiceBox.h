#pragma once

#include "Slate/VerticalBox.h"

#include "Engine/Delegate.h"

#include "Algorithm/Array.h"

#include "ChoiceTypes.h"

class S_Button;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChoceResult, const FChoiceInfos&);

class S_ChoiceBox : public S_VerticalBox
{
public:
	S_ChoiceBox(FVector2D inSize, ID2D1RenderTarget* inD2DRT, const TArray<FChoiceInfos>& inChoiceInfos = {}, FSlateInfos inSlateInfos = FSlateInfos());
	S_ChoiceBox(ID2D1RenderTarget* inD2DRT, const TArray<FChoiceInfos>& inChoiceInfos = {}, FSlateInfos inSlateInfos = FSlateInfos());
	virtual ~S_ChoiceBox();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return GetName() + "_S_ChoiceBox";
		}
		return "S_ChoiceBox";
	}
	FOnChoceResult& GetOnChoiceResult();

	// --------------------------
	// Main : Button
	// --------------------------
	void SetChoiceButton(const TArray<FChoiceInfos>& inChoiceInfos);

protected:
	void OnClicked(DX::MouseEvent inMouseEvent, const FChoiceInfos& inChoiceInfos);

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	FOnChoceResult OnChoiceResult;
};