
#include "Slate/Overlay.h"

void S_Overlay::AddChild(std::shared_ptr<SlateBase> in)
{
	SetSize({ in->GetWidth(), in->GetHeight() });

	SlateContainerBase::AddChild(in);
}

void S_Overlay::SetSize(DirectX::XMFLOAT2 inSize)
{
	m_Size.x = max(inSize.x, m_Size.x);
	m_Size.y = max(inSize.y, m_Size.y);
}

void S_Overlay::Update()
{
	const int numOfChild = (int)m_pChildren.size();
	const float cellH = GetHeight();
	const float cellW = GetWidth();

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

		//OutputDebugStringA(std::format("size {}, {} offset {}, {}\n", NewSize.x, NewSize.y, NewPos.x, NewPos.y).c_str());

		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}
	m_pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Yellow)
	);
}