#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_HorizontalBox : public SlateContainerBase
{
public:
	S_HorizontalBox(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});		
	S_HorizontalBox(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
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
	virtual void Draw() override;
protected:
	virtual void Update() override;
};