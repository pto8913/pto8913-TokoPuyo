
#include "UI/Slate/ProgressBar.h"

#include "Math/Math.h"

#include "Helper/ColorHelper.h"

S_ProgressBar::S_ProgressBar(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateProgressAppearance inAppearance)
	: SlateSlotBase(inSize, inD2DRT, inSlateInfos),
	Appearance(inAppearance)
{
	m_pD2DRenderTarget->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(Appearance.BackgroundColor),
		&m_pBrush
	);

	m_pD2DRenderTarget->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(Appearance.ProgressColor),
		&m_pBrush_Bar
	);
}
S_ProgressBar::S_ProgressBar(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateProgressAppearance inAppearance)
	: S_ProgressBar({0,0}, inD2DRT, inSlateInfos, inAppearance)
{
}

void S_ProgressBar::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	m_pD2DRenderTarget->FillRectangle(
		GetRect(1),
		m_pBrush
	);

	D2D1_RECT_F progressRect = GetRect(Percent);
	m_pD2DRenderTarget->FillRectangle(
		progressRect,
		m_pBrush_Bar
	);
}

D2D1_RECT_F S_ProgressBar::GetRect(float inPercent) const noexcept
{
	return D2D1::RectF(
		m_Position.x + m_Offset.x,
		m_Position.y + m_Offset.y,
		m_Position.x + m_Size.x * inPercent + m_Offset.x,
		m_Position.y + m_Size.y + m_Offset.y
	);
}

void S_ProgressBar::SetPercent(float in)
{
	Percent = Math::Clamp(in, 0.f, 1.f);
}