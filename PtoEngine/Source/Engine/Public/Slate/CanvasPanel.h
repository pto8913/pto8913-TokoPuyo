#pragma once

#include "SlateContainerBase.h"

class S_CanvasPanel : public SlateContainerBase
{
public:
	S_CanvasPanel(ID2D1HwndRenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {});
	S_CanvasPanel(ID2D1HwndRenderTarget* inRt2D, FSlateInfos inSlateInfos = {});
	virtual ~S_CanvasPanel();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_CanvasPanel";
		}
		return "S_CanvasPanel";
	}
	virtual void Update() override;
};