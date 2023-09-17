#pragma once

#include "SlateBase.h"

class SlateSlotBase : public SlateBase
{
public:
	SlateSlotBase(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos = FSlateInfos());
	SlateSlotBase(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos());

	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_SlateSlotBase";
		}
		return "SlateSlotBase";
	}
};