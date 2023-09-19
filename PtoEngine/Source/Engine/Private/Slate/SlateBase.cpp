
#include "Slate/SlateBase.h"

SlateBase::SlateBase(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos)
	: mSize(inSize),
	pRt2D(inRt2D),
	mSlateInfos(inSlateInfos),
	mSlateInputEventReceiveType(ESlateInputEventReceiveType::Enable)
{
	pRt2D->CreateSolidColorBrush(
		D2D1::ColorF(255.f, 255.f, 255.f, 1.f),
		&pBrush
	);
	SetSize(inSize);
}
SlateBase::~SlateBase()
{
	pBrush->Release();

	pRt2D = nullptr;
	pParent = nullptr;
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void SlateBase::SetVisibility(bool in)
{
	bIsVisible = in;
}
bool SlateBase::GetVisibility() const noexcept
{
	return bIsVisible;
}

void SlateBase::UpdateWidget()
{
	Update();
}

SlateBase* SlateBase::GetParent() const noexcept
{
	return pParent;
}
void SlateBase::SetParent(SlateBase* in)
{
	pParent = in;
	AddPosition(in->GetPosition());
}
SlateBase* SlateBase::GetRootParent() noexcept
{
	if (pParent != nullptr)
	{
		return pParent->GetRootParent();
	}
	return this;
}

// ------------------------------------------------
// Main : Event
// ------------------------------------------------
bool SlateBase::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	if (!bIsVisible)
	{
		return false;
	}

	if (InRect(inMouseEvent.x, inMouseEvent.y))
	{
		if (!bIsLastInRect)
		{
			OnMouseEnter(inMouseEvent);
			bIsLastInRect = true;
#if _DEBUG
			//OutputDebugStringA("OnMouseEnter\n");
#endif
		}
	}
	else if (bIsLastInRect)
	{
		OnMouseLeave(inMouseEvent);
		bIsLastInRect = false;
#if _DEBUG
		//OutputDebugStringA("OnMouseLeave\n");
#endif
	}
	return true;
}
bool SlateBase::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	if (!bIsVisible)
	{
		return false;
	}

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
	if (!bIsVisible)
	{
		return false;
	}

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
	if (!bIsVisible)
	{
		return false;
	}

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
	if (!bIsVisible)
	{
		return false;
	}

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
	if (!bIsVisible)
	{
		return false;
	}
	return true;
}
bool SlateBase::OnKeyDown(DX::MouseEvent inMouseEvent)
{
	if (!bIsVisible)
	{
		return false;
	}
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
	if (!bIsVisible)
	{
		return false;
	}
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
	return mSlateInfos;
}
void SlateBase::SetSlateInfos(const FSlateInfos& in)
{
	mSlateInfos = in;
}

void SlateBase::SetHorizontalAlignment(EHorizontalAlignment in)
{
	mSlateInfos.HAlign = in;
}
void SlateBase::SetVerticalAlignment(EVerticalAlignment in)
{
	mSlateInfos.VAlign = in;
}

// ------------------------------------------------
// Main : Transform
// ------------------------------------------------
FRect SlateBase::GetRect() const noexcept
{
	return FRect(
		mPosition.x + mOffset.x,// + mSlateInfos.margin.left,
		mPosition.y + mOffset.y,// + mSlateInfos.margin.top,
		mPosition.x + mSize.x + mOffset.x,// - mSlateInfos.margin.right + mSize.x,
		mPosition.y + mSize.y + mOffset.y// - mSlateInfos.margin.bottom + mSize.y
	);
}
void SlateBase::SetSize(FVector2D inSize)
{
	mSize = inSize;
}
void SlateBase::SetPosition(FVector2D inPosition)
{
	mPosition = inPosition;
}
void SlateBase::AddPosition(FVector2D inPosition)
{
	mPosition += inPosition;
}

bool SlateBase::InRect(float x, float y) const noexcept
{
	const FRect rect = GetRect();
	return x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom;
}
float SlateBase::GetWidth() const noexcept
{
	return mSize.x;
}
float SlateBase::GetHeight() const noexcept
{
	return mSize.y;
}
FVector2D SlateBase::GetSize() const noexcept
{
	return mSize;
}
FVector2D SlateBase::GetPosition() const noexcept
{
	return mPosition;
}
void SlateBase::SetOffset(FVector2D in)
{
	mOffset = in;
}
FVector2D SlateBase::GetOffset() const noexcept
{
	return mOffset;
}
