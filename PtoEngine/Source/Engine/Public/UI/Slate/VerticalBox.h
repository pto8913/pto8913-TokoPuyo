#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_VerticalBox : public SlateContainerBase
{
public:
	S_VerticalBox(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	S_VerticalBox(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	virtual ~S_VerticalBox();

protected:
	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual void Update() override;
};