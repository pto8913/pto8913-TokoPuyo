#pragma once

#include "SlateContainerBase.h"

DECLARE_DELEGATE(FOnClicked);

class DirectX11;

class S_Button : public SlotContainerOnlyOne
{
public:
	S_Button(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	S_Button(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: S_Button(dx, {0,0}, inD2DRT, inSlateInfos)
	{}

	virtual void Draw() override;
	virtual void Update() override {};

	void TryOnClicked(float mouseX, float mouseY);
	FOnClicked OnClicked;
};