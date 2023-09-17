#pragma once

#include "SlateContainerBase.h"

class S_Overlay : public SlateContainerBase
{
public:
	S_Overlay(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	S_Overlay(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
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