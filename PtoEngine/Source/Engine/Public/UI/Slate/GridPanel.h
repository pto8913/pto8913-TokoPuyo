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
	S_GridPanel(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateGridPanelAppearance inAppearance = {});
	S_GridPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateGridPanelAppearance inAppearance = {});
	virtual ~S_GridPanel();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	std::shared_ptr<SlateBase>& GetChildAt(const int& x, const int& y);

	void SetAppearance(const FSlateGridPanelAppearance& in);
	FSlateGridPanelAppearance& GetAppearance();
protected:
	virtual void Update() override;


	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	FSlateGridPanelAppearance mAppearance;
};