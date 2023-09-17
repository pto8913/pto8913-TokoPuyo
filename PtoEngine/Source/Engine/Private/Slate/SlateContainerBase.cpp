
#include "Slate/SlateContainerBase.h"
#include "Slate/SlateSlotBase.h"

#include "Core/DirectX.h"

#include "Helper/RectHelper.h"

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container
// 
//////////////////////////////////////////////////////////////////////////////////////// 

SlateContainerBase::SlateContainerBase(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateBase(inSize, inD2DRT, inSlateInfos)
{
	mSlateInputEventReceiveType = ESlateInputEventReceiveType::NotChildren;
}
SlateContainerBase::SlateContainerBase(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateContainerBase({ 0,0 }, inD2DRT, inSlateInfos)
{
}
SlateContainerBase::~SlateContainerBase()
{
	ClearChildren();
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void SlateContainerBase::Draw()
{
	if (!bIsVisible)
	{
		return;
	}

#if _DEBUG
	pD2DRT->DrawRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
#endif

	for (auto&& child : pChildren)
	{
		child->Draw();
	}
}

void SlateContainerBase::AddChild(std::shared_ptr<SlateBase> in)
{
	in->SetParent(this);
	pChildren.push_back(in);
}
void SlateContainerBase::RemoveChild(int idx)
{
	if (pChildren.size() > idx)
	{
		pChildren[idx].reset();
		pChildren[idx] = nullptr;
	}
	pChildren.erase(pChildren.begin() + idx);
}
void SlateContainerBase::RemoveChild(std::shared_ptr<SlateBase> in)
{
	auto copiedChildren = pChildren;
	int i = 0;
	for (auto&& child : copiedChildren)
	{
		if (pChildren[i] == in)
		{
			pChildren.erase(pChildren.begin() + i);
			break;
		}
		++i;
	}
}
void SlateContainerBase::ClearChildren()
{
#if _DEBUG
	//OutputDebugStringA("---------------------- ClearChildren ----------------------\n");
	//OutputDebugStringA((GetName() + "\n").c_str());
#endif
	for (auto&& child : pChildren)
	{
		child.reset();
		child = nullptr;
	}
#if _DEBUG
	//for (auto&& child : pChildren)
	//{
	//	OutputDebugStringA(("slate children count : " + std::to_string(child.use_count()) + "\n").c_str());
	//}
#endif
	pChildren.erase(pChildren.begin(), pChildren.end());
#if _DEBUG
	//OutputDebugStringA("-----------------------------------------------------------\n");
#endif
}

void SlateContainerBase::UpdateWidget()
{
	Update();
	for (auto&& child : pChildren)
	{
		child->UpdateWidget();
	}
}
void SlateContainerBase::Update()
{
	const int numOfChild = (int)pChildren.size();
	float cellH = GetHeight();
	cellH = cellH == 0 ? pParent->GetHeight() : cellH;
	float cellW = GetWidth();
	cellW = cellW == 0 ? pParent->GetWidth() : cellW;

	FVector2D NewSize = { 0, 0 };
	FVector2D NewPos = { 0, 0 };

	FVector2D SrcPos = mPosition;
	FVector2D SrcSize = mSize;
	//const SlateBase* pRootParent = GetRootParent();
	//if (pParent != nullptr)
	//{
	//	SrcPos = pParent->GetPosition();
	//	SrcSize.x = pParent->GetWidth();
	//	SrcSize.y = pParent->GetHeight();
	//}
	for (int i = 0; i < numOfChild; ++i)
	{
		auto&& pChild = pChildren[i];
		const FSlateInfos childSlateInfos = pChild->GetSlateInfos();
		const float childWidth = pChild->GetWidth();
		const float childHeight = pChild->GetHeight();

		if (childWidth == 0)
		{
			NewSize.x = cellW;
		}
		else
		{
			NewSize.x = childWidth;
		}
		switch (childSlateInfos.HAlign)
		{
		case EHorizontalAlignment::Left:
			NewPos.x = SrcPos.x + childSlateInfos.padding.left;
			break;
		case EHorizontalAlignment::Right:
			NewPos.x = SrcPos.x + SrcSize.x - NewSize.x - childSlateInfos.padding.right;
			break;
		case EHorizontalAlignment::Center:
			NewPos.x = SrcPos.x + (SrcSize.x / 2.f) - (NewSize.x / 2.f) + childSlateInfos.padding.left;
			break;
		default:
			NewSize.x = cellW - childSlateInfos.padding.left - childSlateInfos.padding.right;
			NewPos.x = SrcPos.x + childSlateInfos.padding.left;
			break;
		}

		if (childHeight == 0)
		{
			NewSize.y = cellH;
		}
		else
		{
			NewSize.y = childHeight;
		}
		switch (childSlateInfos.VAlign)
		{
		case EVerticalAlignment::Top:
			NewPos.y = SrcPos.y + childSlateInfos.padding.top;
			break;
		case EVerticalAlignment::Bottom:
			NewPos.y = SrcPos.y + SrcSize.y - NewSize.y - childSlateInfos.padding.bottom;
			break;
		case EVerticalAlignment::Center:
			NewPos.y = SrcPos.y + (cellH / 2.f) - (NewSize.y / 2.f);// +childSlateInfos.padding.top;
			break;
		default:
			NewSize.y = NewSize.y;// -childSlateInfos.padding.top - childSlateInfos.padding.bottom;
			NewPos.y = SrcPos.y + childSlateInfos.padding.top;
			break;
		}

		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}
}
SlateBase* SlateContainerBase::GetChildAt(int idx) const noexcept
{
	if (pChildren.size() > idx)
	{
		return pChildren[idx].get();
	}
	return nullptr;
}

std::vector<std::shared_ptr<SlateBase>> SlateContainerBase::GetChiledren() const noexcept
{
	return pChildren;
}
size_t SlateContainerBase::GetChildrenCount() const noexcept
{
	return pChildren.size();
}
// ------------------------------------------------
// Main : Event
// ------------------------------------------------
bool SlateContainerBase::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	SlateBase::OnMouseMove(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnMouseMove(inMouseEvent);
		}
	}
	return true;
}
bool SlateContainerBase::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseButtonDown(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnMouseButtonDown(inMouseEvent);
		}
	}
	return out;
}
bool SlateContainerBase::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseButtonHeld(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnMouseButtonHeld(inMouseEvent);
		}
	}
	return out;
}
bool SlateContainerBase::OnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseButtonUp(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnMouseButtonUp(inMouseEvent);
		}
	}
	return out;
}
bool SlateContainerBase::OnMouseEnter(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseEnter(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnMouseEnter(inMouseEvent);
		}
	}
	return out;
}
bool SlateContainerBase::OnMouseLeave(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseLeave(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnMouseLeave(inMouseEvent);
		}
	}
	return out;
}
bool SlateContainerBase::OnKeyDown(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnKeyDown(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnKeyDown(inMouseEvent);
		}
	}
	return out;
}
bool SlateContainerBase::OnKeyUp(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnKeyUp(inMouseEvent);
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child->OnKeyUp(inMouseEvent);
		}
	}
	return out;
}

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container only one
// 
//////////////////////////////////////////////////////////////////////////////////////// 
void SlotContainerOnlyOne::AddChild(std::shared_ptr<SlateBase> in)
{
	if (pChildren.size() == 0)
	{
		SlateContainerBase::AddChild(in);
	}
}
SlateBase* SlotContainerOnlyOne::GetChildAt(int) const noexcept
{
	if (pChildren.size() > 0)
	{
		return pChildren[0].get();
	}
	return nullptr;
}

void SlotContainerOnlyOne::Update()
{
	if (GetChildrenCount() == 0) return;

	float cellH = GetHeight();
	cellH = cellH == 0 ? pParent->GetHeight() : cellH;
	float cellW = GetWidth();
	cellW = cellW == 0 ? pParent->GetWidth() : cellW;

	FVector2D NewSize = { 0, 0 };
	FVector2D NewPos = { 0, 0 };

	FVector2D SrcPos = mPosition;
	FVector2D SrcSize = mSize;
	//const SlateBase* pRootParent = GetRootParent();
	//if (pParent != nullptr)
	//{
	//	SrcPos = pParent->GetPosition();
	//	SrcSize.x = pParent->GetWidth();
	//	SrcSize.y = pParent->GetHeight();
	//}

	auto&& pChild = pChildren[0];
	const FSlateInfos childSlateInfos = pChild->GetSlateInfos();
	const float childWidth = pChild->GetWidth();
	const float childHeight = pChild->GetHeight();

	if (childWidth == 0)
	{
		NewSize.x = cellW;
	}
	else
	{
		NewSize.x = childWidth;
	}
	switch (childSlateInfos.HAlign)
	{
	case EHorizontalAlignment::Left:
		NewPos.x = SrcPos.x + childSlateInfos.padding.left;
		break;
	case EHorizontalAlignment::Right:
		NewPos.x = SrcPos.x + SrcSize.x - NewSize.x - childSlateInfos.padding.right;
		break;
	case EHorizontalAlignment::Center:
		NewPos.x = SrcPos.x + (SrcSize.x / 2.f) - (NewSize.x / 2.f) + childSlateInfos.padding.left;
		break;
	default:
		NewSize.x = cellW - childSlateInfos.padding.left - childSlateInfos.padding.right;
		NewPos.x = SrcPos.x + childSlateInfos.padding.left;
		break;
	}

	if (childHeight == 0)
	{
		NewSize.y = cellH;
	}
	else
	{
		NewSize.y = childHeight;
	}
	switch (childSlateInfos.VAlign)
	{
	case EVerticalAlignment::Top:
		NewPos.y = SrcPos.y + childSlateInfos.padding.top;
		break;
	case EVerticalAlignment::Bottom:
		NewPos.y = SrcPos.y + SrcSize.y - NewSize.y - childSlateInfos.padding.bottom;
		break;
	case EVerticalAlignment::Center:
		NewPos.y = SrcPos.y + (cellH / 2.f) - (NewSize.y / 2.f);// +childSlateInfos.padding.top;
		break;
	default:
		NewSize.y = NewSize.y;// -childSlateInfos.padding.top - childSlateInfos.padding.bottom;
		NewPos.y = SrcPos.y + childSlateInfos.padding.top;
		break;
	}

	pChild->SetSize(NewSize);
	pChild->SetPosition(NewPos);
	pChild->Draw();
}