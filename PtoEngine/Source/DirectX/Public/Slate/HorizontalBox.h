#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_HorizontalBox : public SlateContainerBase
{
public:
	S_HorizontalBox(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
	{}
	virtual void Update() override;
};