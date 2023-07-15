#pragma once

#include "SlateContainerBase.h"

class DirectX11;
class SlateSlotBase;

class S_HorizontalBox : public SlateContainerBase
{
public:
	S_HorizontalBox(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});

	virtual void Update() override;
};