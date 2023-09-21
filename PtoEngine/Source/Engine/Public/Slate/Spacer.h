#pragma once

#include "SlateSlotBase.h"

class S_Spacer : public SlateSlotBase
{
public:
	S_Spacer(ID2D1HwndRenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {});
	S_Spacer(ID2D1HwndRenderTarget* inRt2D, FSlateInfos inSlateInfos = {});

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