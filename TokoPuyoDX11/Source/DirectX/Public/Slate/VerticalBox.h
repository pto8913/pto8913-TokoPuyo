#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_VerticalBox : public SlateContainerBase
{
public:
	S_VerticalBox(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
	{}
	virtual void Update() override;
};