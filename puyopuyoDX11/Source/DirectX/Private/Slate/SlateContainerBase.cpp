
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

void SlateContainerBase::AddChild(SlateBase* in)
{
	in->SetParent(this);
	m_pChildren.push_back(in);
	for (auto&& child : m_pChildren)
	{
		child->Update();
	}
	Update();
}
SlateBase* SlateContainerBase::GetSlotAt(int idx) const noexcept
{
	if (m_pChildren.size() > idx)
	{
		return m_pChildren[idx];
	}
	return nullptr;
}

std::vector<SlateBase*> SlateContainerBase::GetChiledren() const noexcept
{
	return m_pChildren;
}
size_t SlateContainerBase::GetChildrenCount() const noexcept
{
	return m_pChildren.size();
}

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container only one
// 
//////////////////////////////////////////////////////////////////////////////////////// 
void SlotContainerOnlyOne::AddChild(SlateBase* in)
{
	if (m_pChildren.size() == 0)
	{
		SlateContainerBase::AddChild(in);
	}
}
SlateBase* SlotContainerOnlyOne::GetChild() const noexcept
{
	if (m_pChildren.size() > 0)
	{
		return m_pChildren[0];
	}
	return nullptr;
}