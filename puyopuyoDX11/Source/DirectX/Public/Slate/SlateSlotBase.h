#pragma once

#include "SlateBase.h"

class SlateSlotBase : public SlateBase
{
public:
	SlateSlotBase(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos())
		: SlateBase(dx, inSize, inD2DRT, inSlateInfos)
	{}
	SlateSlotBase(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos())
		: SlateBase(dx, { 0,0 }, inD2DRT, inSlateInfos)
	{}
};