#pragma once

#include "SlateContainerBase.h"

class S_CanvasPanel : public SlateContainerBase
{
public:
	S_CanvasPanel(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	S_CanvasPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	virtual ~S_CanvasPanel();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual void Update() override;
};