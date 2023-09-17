#pragma once

#include "SlateBase.h"

class SlateSlotBase : public SlateBase
{
public:
	SlateSlotBase(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos())
		: SlateBase(inSize, inD2DRT, inSlateInfos)
	{}
	SlateSlotBase(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos())
		: SlateBase({ 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return GetName() + "_SlateSlotBase";
		}
		return "SlateSlotBase";
	}
};