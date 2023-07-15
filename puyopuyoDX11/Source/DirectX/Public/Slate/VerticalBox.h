#pragma once

#include "SlateContainerBase.h"

class DirectX11;
class SlateSlotBase;

class S_VerticalBox : public SlateContainerBase
{
public:
	S_VerticalBox(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(dx, inSize, inD2DRT, inSlateInfos)
	{}
	virtual void Update() override;
};