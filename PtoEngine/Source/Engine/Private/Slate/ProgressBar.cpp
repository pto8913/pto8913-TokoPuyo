
#include "Slate/ProgressBar.h"

#include "Math/Math.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

S_ProgressBar::S_ProgressBar(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos, FSlateProgressAppearance inAppearance)
	: SlateSlotBase(inD2DRT, inSize, inSlateInfos),
	mAppearance(inAppearance),
	mPercent(1.f)
{
	pD2DRT->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.BackgroundColor),
		&pBrush
	);

	pD2DRT->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.ProgressColor),
		&pBarBrush
	);
}
S_ProgressBar::S_ProgressBar(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateProgressAppearance inAppearance)
	: S_ProgressBar(inD2DRT, { 0,0 }, inSlateInfos, inAppearance)
{
}
S_ProgressBar::~S_ProgressBar()
{
	Util::SafeRelease(pBarBrush);
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void S_ProgressBar::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	pD2DRT->FillRectangle(
		RectHelper::ConvertRectToD2D(GetRect(1)),
		pBrush
	);

	FRect progressRect = GetRect(mPercent);
	pD2DRT->FillRectangle(
		RectHelper::ConvertRectToD2D(progressRect),
		pBarBrush
	);
}

FRect S_ProgressBar::GetRect(float inPercent) const noexcept
{
	return FRect(
		mPosition.x + mOffset.x,
		mPosition.y + mOffset.y,
		mPosition.x + mSize.x * inPercent + mOffset.x,
		mPosition.y + mSize.y + mOffset.y
	);
}

void S_ProgressBar::SetPercent(float in)
{
	mPercent = Math::Clamp(in, 0.f, 1.f);
}

void S_ProgressBar::SetAppearance(const FSlateProgressAppearance& in)
{
	mAppearance = in;
}
FSlateProgressAppearance& S_ProgressBar::GetAppearance()
{
	return mAppearance;
}