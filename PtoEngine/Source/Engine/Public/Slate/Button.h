#pragma once

#include "SlateContainerBase.h"

#include "Engine/Color.h"

struct FSlateButtonAppearance
{
public:
	FSlateButtonAppearance()
		: DefaultColor(FColor(0.5f, 0.5f, 0.5f)),
		HoverColor(FColor(0.75f, 0.75f, 0.75f)),
		PressColor(FColor(0.25f, 0.25f, 0.25f))
	{}
	FColor DefaultColor;
	FColor HoverColor;
	FColor PressColor;
};

class S_Button : public SlotContainerOnlyOne
{
public:
	S_Button(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	S_Button(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	virtual ~S_Button();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_Button";
		}
		return "S_Button";
	}
	virtual void Draw() override;

	void SetAppearance(const FSlateButtonAppearance& in);
	FSlateButtonAppearance& GetAppearance();

	// ------------------------------------------------
	// Main : Event
	// ------------------------------------------------
	virtual bool OnMouseButtonDown(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonHeld(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonUp(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseEnter(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseLeave(DX::MouseEvent inMouseEvent) override;

	DX::FOnMouseAction OnClicked;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	FSlateButtonAppearance mAppearance;
};