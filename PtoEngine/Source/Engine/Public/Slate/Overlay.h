#pragma once

#include "SlateContainerBase.h"

class S_Overlay : public SlateContainerBase
{
public:
	S_Overlay(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {});
	S_Overlay(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos = {});
	virtual ~S_Overlay();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_Overlay";
		}
		return "S_Overlay";
	}
	virtual void Draw() override;

	virtual void SetSize(FVector2D inSize) override;

	virtual void AddChild(std::shared_ptr<SlateBase> in) override;

protected:
	virtual void Update() override;
};