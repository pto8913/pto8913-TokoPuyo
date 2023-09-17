#pragma once

#include "SlateSlotBase.h"

class S_Spacer : public SlateSlotBase
{
public:
	S_Spacer(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateSlotBase(inSize, inD2DRT, inSlateInfos)
	{}
	S_Spacer(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: S_Spacer({ 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual void Draw() override {};

	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_Spacer";
		}
		return "S_Spacer";
	}
};