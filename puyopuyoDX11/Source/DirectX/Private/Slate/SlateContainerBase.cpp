
#include "Slate/SlateContainerBase.h"
#include "Slate/SlateSlotBase.h"

#include "Core/DirectX.h"

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container
// 
//////////////////////////////////////////////////////////////////////////////////////// 
void SlateContainerBase::Draw()
{
	m_pD2DRenderTarget->DrawRectangle(
		GetRect(), m_pBrush
	);
	for (auto&& child : m_pChildren)
	{
		child->Draw();
	}
}

void SlateContainerBase::AddChild(std::shared_ptr<SlateBase> in)
{
	in->SetParent(this);
	m_pChildren.push_back(in);
	for (auto&& child : m_pChildren)
	{
		child->Update();
	}
	Update();
}
void SlateContainerBase::RemoveChild(int idx)
{
	if (m_pChildren.size() > idx)
	{
		m_pChildren[idx].reset();
	}
	m_pChildren.erase(m_pChildren.begin() + idx);
	for (auto&& child : m_pChildren)
	{
		child->Update();
	}
	Update();
}
void SlateContainerBase::ClearChildren()
{
	for (auto&& child : m_pChildren)
	{
		child.reset();
	}
	m_pChildren.erase(m_pChildren.begin(), m_pChildren.end());
}

void SlateContainerBase::Update()
{
	const int numOfChild = (int)m_pChildren.size();
	float cellH = GetHeight();
	cellH = cellH == 0 ? m_pParent->GetHeight() : cellH;
	float cellW = GetWidth();
	cellW = cellW == 0 ? m_pParent->GetWidth() : cellW;

	DirectX::XMFLOAT2 NewSize = { 0, 0 };
	DirectX::XMFLOAT2 NewPos = { 0, 0 };

	DirectX::XMFLOAT2 SrcPos = m_Position;
	DirectX::XMFLOAT2 SrcSize = m_Size;
	//const SlateBase* pRootParent = GetRootParent();
	//if (m_pParent != nullptr)
	//{
	//	SrcPos = m_pParent->GetPosition();
	//	SrcSize.x = m_pParent->GetWidth();
	//	SrcSize.y = m_pParent->GetHeight();
	//}
	for (int i = 0; i < numOfChild; ++i)
	{
		auto&& pChild = m_pChildren[i];
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
SlateBase* SlateContainerBase::GetSlotAt(int idx) const noexcept
{
	if (m_pChildren.size() > idx)
	{
		return m_pChildren[idx].get();
	}
	return nullptr;
}

std::vector<std::shared_ptr<SlateBase>> SlateContainerBase::GetChiledren() const noexcept
{
	return m_pChildren;
}
size_t SlateContainerBase::GetChildrenCount() const noexcept
{
	return m_pChildren.size();
}
// ------------------------------------------------
// Main : Event
// ------------------------------------------------
bool SlateContainerBase::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	SlateBase::OnMouseMove(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnMouseMove(inMouseEvent);
	}
	return true;
}
bool SlateContainerBase::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseButtonDown(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnMouseButtonDown(inMouseEvent);
	}
	return out;
}
bool SlateContainerBase::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseButtonHeld(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnMouseButtonHeld(inMouseEvent);
	}
	return out;
}
bool SlateContainerBase::OnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseButtonUp(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnMouseButtonUp(inMouseEvent);
	}
	return out;
}
bool SlateContainerBase::OnMouseEnter(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseEnter(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnMouseEnter(inMouseEvent);
	}
	return out;
}
bool SlateContainerBase::OnMouseLeave(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnMouseLeave(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnMouseLeave(inMouseEvent);
	}
	return out;
}
bool SlateContainerBase::OnKeyDown(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnKeyDown(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnKeyDown(inMouseEvent);
	}
	return out;
}
bool SlateContainerBase::OnKeyUp(DX::MouseEvent inMouseEvent)
{
	bool out = SlateBase::OnKeyUp(inMouseEvent);
	for (auto&& child : m_pChildren)
	{
		child->OnKeyUp(inMouseEvent);
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
	if (m_pChildren.size() == 0)
	{
		SlateContainerBase::AddChild(in);
	}
}
SlateBase* SlotContainerOnlyOne::GetSlotAt(int idx) const noexcept
{
	if (m_pChildren.size() > 0)
	{
		return m_pChildren[0].get();
	}
	return nullptr;
}