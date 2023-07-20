
#include "Slate/SlateBase.h"

SlateBase::SlateBase(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: m_Size(inSize), 
	m_pD2DRenderTarget(inD2DRT),
	m_SlateInfos(inSlateInfos),
	mSlateInputEventReceiveType(ESlateInputEventReceiveType::Enable)
{
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
// Main : Event
// ------------------------------------------------
bool SlateBase::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	if (InRect(inMouseEvent.x, inMouseEvent.y))
	{
		if (!bIsLastInRect)
		{
			OnMouseEnter(inMouseEvent);
			bIsLastInRect = true;
#if _DEBUG
			OutputDebugStringA("OnMouseEnter\n");
#endif
		}
	}
	else if (bIsLastInRect)
	{
		OnMouseLeave(inMouseEvent);
		bIsLastInRect = false;
#if _DEBUG
		OutputDebugStringA("OnMouseLeave\n");
#endif
	}
	return true;
}
bool SlateBase::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	switch (mSlateInputEventReceiveType)
	{
	case ESlateInputEventReceiveType::NotAll:
		return false;
		break;
	default:
		auto rect = GetRect();
		if (inMouseEvent.State == DX::MouseEvent::ButtonState::LPRESSED ||
			inMouseEvent.State == DX::MouseEvent::ButtonState::RPRESSED
			)
		{
			if (InRect(inMouseEvent.x, inMouseEvent.y))
			{
				return true;
			}
		}
		return false;
	}
}
bool SlateBase::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	switch (mSlateInputEventReceiveType)
	{
	case ESlateInputEventReceiveType::NotAll:
		return false;
	default:
		auto rect = GetRect();
		if (inMouseEvent.State == DX::MouseEvent::ButtonState::LHELD ||
			inMouseEvent.State == DX::MouseEvent::ButtonState::RHELD
			)
		{
			if (InRect(inMouseEvent.x, inMouseEvent.y))
			{
				return true;
			}
		}
		return false;
	}
}
bool SlateBase::OnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	switch (mSlateInputEventReceiveType)
	{
	case ESlateInputEventReceiveType::NotAll:
		return false;
		break;
	default:
		auto rect = GetRect();
		if (inMouseEvent.State == DX::MouseEvent::ButtonState::LRELEASED ||
			inMouseEvent.State == DX::MouseEvent::ButtonState::RRELEASED
			)
		{
			if (InRect(inMouseEvent.x, inMouseEvent.y))
			{
				return true;
			}
		}
		return false;
	}
}
bool SlateBase::OnMouseEnter(DX::MouseEvent inMouseEvent)
{
	switch (mSlateInputEventReceiveType)
	{
	case ESlateInputEventReceiveType::NotAll:
		return false;
		break;
	default:
		return InRect(inMouseEvent.x, inMouseEvent.y);
	}
}
bool SlateBase::OnMouseLeave(DX::MouseEvent)
{
	return true;
}
bool SlateBase::OnKeyDown(DX::MouseEvent inMouseEvent)
{
	switch (mSlateInputEventReceiveType)
	{
	case ESlateInputEventReceiveType::NotAll:
		return false;
		break;
	default:
		return InRect(inMouseEvent.x, inMouseEvent.y);
	}
}
bool SlateBase::OnKeyUp(DX::MouseEvent inMouseEvent)
{
	switch (mSlateInputEventReceiveType)
	{
	case ESlateInputEventReceiveType::NotAll:
		return false;
		break;
	default:
		return InRect(inMouseEvent.x, inMouseEvent.y);
	}
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
	return x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom;
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
