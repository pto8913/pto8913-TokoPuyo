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
	S_Button(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	S_Button(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	virtual ~S_Button();

	virtual void Draw() override;

	// ------------------------------------------------
	// Main : Event
	// ------------------------------------------------
	virtual bool OnMouseButtonDown(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonHeld(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonUp(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseEnter(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseLeave(DX::MouseEvent inMouseEvent) override;

	DX::FOnMouseAction OnClicked;

	FSlateButtonAppearance ButtonAppearance;
};