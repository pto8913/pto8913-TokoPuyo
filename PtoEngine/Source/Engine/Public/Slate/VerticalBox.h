#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_VerticalBox : public SlateContainerBase
{
public:
	S_VerticalBox(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos = {});
	S_VerticalBox(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	virtual ~S_VerticalBox();

	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_VerticalBox";
		}
		return "S_VerticalBox";
	}
protected:
	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual void Update() override;
};