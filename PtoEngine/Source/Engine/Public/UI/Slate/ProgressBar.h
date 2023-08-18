#pragma once

#include "SlateSlotBase.h"
#include "Engine/Color.h"

struct FSlateProgressAppearance
{
public:
	FSlateProgressAppearance()
		: BackgroundColor(FColor(0.5f, 0.5f, 0.5f)),
		ProgressColor(FColor(0.75f, 0.75f, 0.75f))
	{}

	FColor BackgroundColor;
	FColor ProgressColor;
};


class S_ProgressBar : public SlateSlotBase
{
public:
	S_ProgressBar(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateProgressAppearance inAppearance = {});
	S_ProgressBar(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateProgressAppearance inAppearance = {});
	virtual void Draw() override;

	D2D1_RECT_F GetRect(float inPercent) const noexcept;

	void SetPercent(float in);
private:
	FSlateProgressAppearance Appearance;

	float Percent;

	ID2D1SolidColorBrush* m_pBrush_Bar;
};