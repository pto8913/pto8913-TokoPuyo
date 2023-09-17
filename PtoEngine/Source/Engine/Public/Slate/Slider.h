#pragma once

#include "SlateSlotBase.h"

#include "Engine/Color.h"

class DirectX11;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChanged, float);

enum ESliderDirection
{
	Horizontal,
	Vertical
};

struct FSlateSliderAppearance
{
public:
	FSlateSliderAppearance()
		: 
		sliderButtonSize({ 5.f, 5.f }),
		sliderButtonColor(255.f, 255.f, 255.f, 1.f),
		step(0.1f),
		sliderLineThickness(1.f),
		sliderLineColor(100.f, 100.f, 100.f, 1.f),
		direction(ESliderDirection::Horizontal)
	{}
	/* Slider Button */
	FVector2D sliderButtonSize;
	FColor sliderButtonColor;
	float step;

	/* Slider Line */
	float sliderLineThickness;
	FColor sliderLineColor;

	ESliderDirection direction;
};

class S_Slider : public SlateSlotBase
{
public:
	S_Slider(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateSliderAppearance inAppearance = {});
	S_Slider(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateSliderAppearance inAppearance = {});
	virtual ~S_Slider();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return GetName() + "_S_Slider";
		}
		return "S_Slider";
	}
	virtual void Draw() override;

	void SetAppearance(FSlateSliderAppearance inAppearance);

	void SetMaxValue(float in);
	void SetMinValue(float in);
	void SetValue(float in);

	virtual bool OnMouseButtonDown(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonHeld(DX::MouseEvent inMouseEvent) override;
protected:
	float GetSliderPosition();
public:
	// --------------------------
	// Main : Delegates
	// --------------------------
	FOnValueChanged OnValueChanged;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	float mMaxValue = 1.f;
	float mMinValue = 0.f;
	float mCurrent = 0.5f;

	FSlateSliderAppearance mAppearance;

	ID2D1SolidColorBrush* pLineBrush = nullptr;
};