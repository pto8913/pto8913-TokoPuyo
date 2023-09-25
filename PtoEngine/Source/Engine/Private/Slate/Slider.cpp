
#include "Slate/Slider.h"

#include "Graphics/Core/DirectX.h"

#include "Utils/String.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

#include "Algorithm/Math.h"
#include "Algorithm/Math.h"

#if _DEBUG
#include <format>
#endif

S_Slider::S_Slider(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos, FSlateSliderAppearance inAppearance)
	: SlateSlotBase(inRt2D, inSize, inSlateInfos), 
	mAppearance(inAppearance)
{
	inRt2D->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.sliderLineColor),
		&pLineBrush
	);

	SetAppearance(inAppearance);
}
S_Slider::S_Slider(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos, FSlateSliderAppearance inAppearance)
	: S_Slider(inRt2D, { 0,0 }, inSlateInfos, inAppearance)
{
}

S_Slider::~S_Slider()
{
	OnValueChanged.ClearBind();
	
	Util::SafeRelease(pLineBrush);
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void S_Slider::Draw()
{
	switch (GetVisibility())
	{
	case ESlateVisibility::Collapsed:
		return;
	default:
		switch (mAppearance.direction)
		{
		case ESliderDirection::Horizontal:
			/* Draw SliderLine */
			pRt2D->FillRectangle(
				RectHelper::ConvertRectToD2D(
					FRect(
						mPosition.x + mOffset.x,
						mPosition.y + mOffset.y + mSize.y / 2 - mAppearance.sliderLineThickness / 2,
						mPosition.x + mOffset.x + mSize.x,
						mPosition.y + mOffset.y + mSize.y / 2 + mAppearance.sliderLineThickness / 2
					)
				),
				pLineBrush
			);
			/* Draw Slider */
			pRt2D->FillRectangle(
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
			pRt2D->FillRectangle(
				RectHelper::ConvertRectToD2D(
					FRect(
						mPosition.x + mOffset.x - mAppearance.sliderLineThickness / 2,
						mPosition.y + mOffset.y,
						mPosition.x + mOffset.x + mAppearance.sliderLineThickness / 2,
						mPosition.y + mOffset.y + mSize.y
					)
				),
				pLineBrush
			);
			/* Draw Slider */
			pRt2D->FillRectangle(
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
		break;
	}
#if _DEBUG
	pRt2D->DrawRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
#endif
}

void S_Slider::SetAppearance(FSlateSliderAppearance inAppearance)
{
	mAppearance = inAppearance;

	pLineBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.sliderLineColor));
	pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.sliderButtonColor));
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
	if (GetVisibility() == ESlateVisibility::Visible)
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
	}
	return false;
}
bool S_Slider::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	if (GetVisibility() == ESlateVisibility::Visible)
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
