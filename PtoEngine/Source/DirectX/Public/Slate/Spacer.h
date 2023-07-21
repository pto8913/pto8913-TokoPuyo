#pragma once

#include "SlateSlotBase.h"

class S_Spacer : public SlateSlotBase
{
public:
	S_Spacer(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateSlotBase(inSize, inD2DRT, inSlateInfos)
	{}
	S_Spacer(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: S_Spacer({ 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual void Draw() override {};
};