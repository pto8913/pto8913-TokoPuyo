#pragma once

#include "SlateContainerBase.h"

struct FSlateGridPanelAppearance
{
public:
	FSlateGridPanelAppearance()
	{}

	int column = 2;
	int row = 2;
};

class S_GridPanel : public SlateContainerBase
{
public:
	S_GridPanel(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {}, FSlateGridPanelAppearance inAppearance = {});
	S_GridPanel(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos = {}, FSlateGridPanelAppearance inAppearance = {});
	virtual ~S_GridPanel();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_GridPanel";
		}
		return "S_GridPanel";
	}
	SlateBase* GetChildAt(const int& x, const int& y);

	void SetAppearance(const FSlateGridPanelAppearance& in);
	FSlateGridPanelAppearance& GetAppearance();
protected:
	virtual void Update() override;


	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	FSlateGridPanelAppearance mAppearance;
};