#pragma once

#include "SlateContainerBase.h"

struct FSlateButtonAppearance
{
public:
	FSlateButtonAppearance()
		: DefaultColor(D2D1::ColorF(0.5f, 0.5f, 0.5f)),
		HoverColor(D2D1::ColorF(0.75f, 0.75f, 0.75f)),
		PressColor(D2D1::ColorF(0.25f, 0.25f, 0.25f))
	{}
	D2D1_COLOR_F DefaultColor;
	D2D1_COLOR_F HoverColor;
	D2D1_COLOR_F PressColor;
};

class S_Button : public SlotContainerOnlyOne
{
public:
	S_Button(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {});
	S_Button(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateButtonAppearance inButtonAppearance = {})
		: S_Button({0,0}, inD2DRT, inSlateInfos, inButtonAppearance)
	{}

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