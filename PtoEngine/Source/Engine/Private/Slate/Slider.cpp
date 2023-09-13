
#include "Slate/Slider.h"

#include "Core/DirectX.h"

#include "Utils/String.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

#include "Math/Math.h"
#include "Algorithm/Math2.h"

#if _DEBUG
#include <format>
#endif

S_Slider::S_Slider(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateSliderAppearance inAppearance)
	: SlateSlotBase(inSize, inD2DRT, inSlateInfos), 
	mAppearance(inAppearance)
{
}
S_Slider::S_Slider(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateSliderAppearance inAppearance)
	: S_Slider({ 0,0 }, inD2DRT, inSlateInfos, inAppearance)
{
}

S_Slider::~S_Slider()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void S_Slider::Draw()
{
	if (!bIsVisible)
	{
		return;
	}

	switch (mAppearance.direction)
	{
	case ESliderDirection::Horizontal:
		/* Draw SliderLine */
		pD2DRT->FillRectangle(
			RectHelper::ConvertRectToD2D(
				FRect(
					mPosition.x + mOffset.x,
					mPosition.y + mOffset.y + mSize.y / 2 - mAppearance.sliderLineThickness / 2,
					mPosition.x + mOffset.x + mSize.x,
					mPosition.y + mOffset.y + mSize.y / 2 + mAppearance.sliderLineThickness / 2
				)
			),
			pBrush
		);
		/* Draw Slider */
		pD2DRT->FillRectangle(
			RectHelper::ConvertRectToD2D(
				FRect(
					mPosition.x + mOffset.x - mAppearance.sliderButtonSize.x / 2 + GetSliderPosition(),
					mPosition.y + mOffset.y - mAppearance.sliderButtonSize.y / 2 + mSize.y / 2,
					mPosition.x + mOffset.x + mAppearance.sliderButtonSize.x / 2 + GetSliderPosition(),
					mPosition.y + mOffset.y + mAppearance.sliderButtonSize.y / 2 + mSize.y / 2
				)
			),
			pBrush
		);
		break;
	default:
		/* Draw SliderLine */
		pD2DRT->FillRectangle(
			RectHelper::ConvertRectToD2D(
				FRect(
					mPosition.x + mOffset.x - mAppearance.sliderLineThickness / 2,
					mPosition.y + mOffset.y,
					mPosition.x + mOffset.x + mAppearance.sliderLineThickness / 2,
					mPosition.y + mOffset.y + mSize.y
				)
			),
			pBrush
		);
		/* Draw Slider */
		pD2DRT->FillRectangle(
			RectHelper::ConvertRectToD2D(
				FRect(
					mPosition.x + mOffset.x - mAppearance.sliderButtonSize.x / 2,
					mPosition.y + mOffset.y - mAppearance.sliderButtonSize.y / 2 + GetSliderPosition(),
					mPosition.x + mOffset.x + mAppearance.sliderButtonSize.x / 2,
					mPosition.y + mOffset.y + mAppearance.sliderButtonSize.y / 2 + GetSliderPosition()
				)
			),
			pBrush
		);
		break;
	}

#if _DEBUG
	pD2DRT->DrawRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
#endif
}

void S_Slider::SetAppearance(FSlateSliderAppearance inAppearance)
{
	mAppearance = inAppearance;
}

void S_Slider::SetMaxValue(float in)
{
	mMaxValue = in;
}
void S_Slider::SetMinValue(float in)
{
	mMinValue = in;
}
void S_Slider::SetValue(float in)
{
	mCurrent = Math::Clamp(in, mMinValue, mMaxValue);
	OnValueChanged.Broadcast(mCurrent);
#if _DEBUG
	OutputDebugStringA(std::format("Slider Value {}\n", mCurrent).c_str());
#endif
}

bool S_Slider::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	if (inMouseEvent.State == DX::MouseEvent::ButtonState::LPRESSED)
	{
		if (InRect(inMouseEvent.x, inMouseEvent.y))
		{
			switch (mAppearance.direction)
			{
			case ESliderDirection::Horizontal:
#if _DEBUG
				OutputDebugStringA(std::format("Slider RangeValue {} {} {} {} {}\n", (float)inMouseEvent.x - mPosition.x, 0.f, mSize.x, mMinValue, mMaxValue).c_str());
#endif
				SetValue(Math::MapRange((float)inMouseEvent.x - mPosition.x, 0.f, mSize.x, mMinValue, mMaxValue));
				break;
			default:
				SetValue(Math::MapRange((float)inMouseEvent.y - mPosition.y, 0.f, mSize.y, mMinValue, mMaxValue));
				break;
			}
			return true;
		}
	}
	return false;
}
bool S_Slider::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	if (inMouseEvent.State == DX::MouseEvent::ButtonState::LHELD)
	{
		if (InRect(inMouseEvent.x, inMouseEvent.y))
		{
			switch (mAppearance.direction)
			{
			case ESliderDirection::Horizontal:
#if _DEBUG
				OutputDebugStringA(std::format("Slider RangeValue {} {} {} {} {}\n", (float)inMouseEvent.x - mPosition.x, 0.f, mSize.x, mMinValue, mMaxValue).c_str());
#endif
				SetValue(Math::MapRange((float)inMouseEvent.x - mPosition.x, 0.f, mSize.x, mMinValue, mMaxValue));
				break;
			default:
				SetValue(Math::MapRange((float)inMouseEvent.y - mPosition.y, 0.f, mSize.y, mMinValue, mMaxValue));
				break;
			}
			return true;
		}
	}
	return false;
}

float S_Slider::GetSliderPosition()
{
	switch (mAppearance.direction)
	{
	case ESliderDirection::Horizontal:
		return GetSize().x * mCurrent / mMaxValue;
	default:
		return GetSize().y * mCurrent / mMaxValue;
	}
}
