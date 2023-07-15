
#include "Slate/SlateBase.h"

SlateBase::SlateBase(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: m_Size(inSize), m_pD2DRenderTarget(inD2DRT), m_SlateInfos(inSlateInfos)
{
	m_pContext = dx.GetContext2D();
	m_pD2DRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(255.f, 255.f, 255.f, 1.f),
		&m_pBrush
	);
	SetSize(inSize);
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
SlateBase* SlateBase::GetParent() const noexcept
{
	return m_pParent;
}
void SlateBase::SetParent(SlateBase* in)
{
	m_pParent = in;
}
SlateBase* SlateBase::GetRootParent() noexcept
{
	if (m_pParent != nullptr)
	{
		return m_pParent->GetRootParent();
	}
	return this;
}

// ------------------------------------------------
// Main : SlateInfos
// ------------------------------------------------
FSlateInfos SlateBase::GetSlateInfos() const noexcept
{
	return m_SlateInfos;
}
void SlateBase::SetSlateInfos(const FSlateInfos& in)
{
	m_SlateInfos = in;
}

void SlateBase::SetHorizontalAlignment(EHorizontalAlignment in)
{
	m_SlateInfos.HAlign = in;
}
void SlateBase::SetVerticalAlignment(EVerticalAlignment in)
{
	m_SlateInfos.VAlign = in;
}

// ------------------------------------------------
// Main : Transform
// ------------------------------------------------
D2D1_RECT_F SlateBase::GetRect() const noexcept
{
	return D2D1::RectF(
		m_Position.x + m_Offset.x,// + m_SlateInfos.margin.left,
		m_Position.y + m_Offset.y,// + m_SlateInfos.margin.top,
		m_Position.x + m_Size.x + m_Offset.x,// - m_SlateInfos.margin.right + m_Size.x,
		m_Position.y + m_Size.y + m_Offset.y// - m_SlateInfos.margin.bottom + m_Size.y
	);
}
void SlateBase::SetRect(DirectX::XMFLOAT2 inSize, DirectX::XMFLOAT2 inOffset)
{
	m_Rect = D2D1::RectF(
		inOffset.x,// + m_SlateInfos.margin.left,
		inOffset.y,// + m_SlateInfos.margin.top,
		inOffset.x + inSize.x,// - m_SlateInfos.margin.right + inSize.x,
		inOffset.y + inSize.y// - m_SlateInfos.margin.bottom + m_Size.y
	);
}
void SlateBase::SetSize(DirectX::XMFLOAT2 inSize)
{
	m_Size = inSize;
}
void SlateBase::SetPosition(DirectX::XMFLOAT2 inPosition)
{
	m_Position = inPosition;
}

bool SlateBase::InRect(float x, float y) const noexcept
{
	const D2D1_RECT_F rect = GetRect();
	return x >= rect.left && x <= rect.right && y >= rect.bottom && y <= rect.top;
}
float SlateBase::GetWidth() const noexcept
{
	return m_Size.x;
}
float SlateBase::GetHeight() const noexcept
{
	return m_Size.y;
}
DirectX::XMFLOAT2 SlateBase::GetSize() const noexcept
{
	return m_Size;
}
DirectX::XMFLOAT2 SlateBase::GetPosition() const noexcept
{
	return m_Position;
}
void SlateBase::SetOffset(DirectX::XMFLOAT2 in)
{
	m_Offset = in;
}
DirectX::XMFLOAT2 SlateBase::GetOffset() const noexcept
{
	return m_Offset;
}
