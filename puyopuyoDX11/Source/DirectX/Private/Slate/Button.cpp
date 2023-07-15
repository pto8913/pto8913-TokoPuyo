
#include "Slate/Button.h"

S_Button::S_Button(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlotContainerOnlyOne(dx, inSize, inD2DRT, inSlateInfos)
{
	m_pD2DRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(50.f, 50.f, 50.f, 1.f),
		&m_pBrush
	);
}

void S_Button::Draw()
{
	//m_pD2DRenderTarget->DrawRectangle(
	//	GetRect(),
	//	m_pBrush
	//);
	m_pD2DRenderTarget->FillRectangle(
		GetRect(),
		m_pBrush
	);
}

void S_Button::TryOnClicked(float mouseX, float mouseY)
{
	auto rect = GetRect();
	if (InRect(mouseX, mouseY))
	{
		OnClicked.Broadcast();
	}
}