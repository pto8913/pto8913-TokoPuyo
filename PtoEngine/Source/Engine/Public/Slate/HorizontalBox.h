#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_HorizontalBox : public SlateContainerBase
{
public:
	S_HorizontalBox(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {});		
	S_HorizontalBox(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos = {});
	virtual ~S_HorizontalBox();
	
	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_HorizontalBox";
		}
		return "S_HorizontalBox";
	}
protected:
	virtual void Update() override;
};