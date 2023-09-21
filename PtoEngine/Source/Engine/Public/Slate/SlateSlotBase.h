#pragma once

#include "SlateBase.h"

class SlateSlotBase : public SlateBase
{
public:
	SlateSlotBase(ID2D1HwndRenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = FSlateInfos());
	SlateSlotBase(ID2D1HwndRenderTarget* inRt2D, FSlateInfos inSlateInfos = FSlateInfos());

	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_SlateSlotBase";
		}
		return "SlateSlotBase";
	}
};