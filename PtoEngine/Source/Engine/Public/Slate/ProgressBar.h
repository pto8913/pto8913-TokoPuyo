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
	S_ProgressBar(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos = {}, FSlateProgressAppearance inAppearance = {});
	S_ProgressBar(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos = {}, FSlateProgressAppearance inAppearance = {});
	virtual ~S_ProgressBar();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_ProgressBar";
		}
		return "S_ProgressBar";
	}
	virtual void Draw() override;

	FRect GetRect(float inPercent) const noexcept;

	void SetPercent(float in);

	void SetAppearance(const FSlateProgressAppearance& in);
	FSlateProgressAppearance& GetAppearance();
private:
	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	ID2D1SolidColorBrush* pBarBrush = nullptr;

	FSlateProgressAppearance mAppearance;
	float mPercent;
};