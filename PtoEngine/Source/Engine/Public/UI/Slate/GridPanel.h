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
	S_GridPanel(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateGridPanelAppearance inAppearance = {});
	S_GridPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateGridPanelAppearance inAppearance = {});

	std::shared_ptr<SlateBase>& GetChildAt(const int& x, const int& y);
	void SetAppearance(const FSlateGridPanelAppearance& in);
protected:
	virtual void Update() override;

	FSlateGridPanelAppearance appearance;
};