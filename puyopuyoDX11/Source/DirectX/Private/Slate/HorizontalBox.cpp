
#include "Slate/HorizontalBox.h"

S_HorizontalBox::S_HorizontalBox(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateContainerBase(dx, inSize, inD2DRT, inSlateInfos)
{

}
void S_HorizontalBox::Update()
{
	const D2D1_RECT_F containerRect = GetRect();

	const int numOfChild = (int)m_pChildren.size();
	const float cellH = GetHeight() / numOfChild;
	const float cellW = GetWidth();

	DirectX::XMFLOAT2 NewSize = { 0, 0 };
	DirectX::XMFLOAT2 NewPos = { 0, 0 };
	float accumulatePosX = 0.f;

	DirectX::XMFLOAT2 SrcPos = m_Position;
	DirectX::XMFLOAT2 SrcSize = m_Size;
	//const SlateBase* pRootParent = GetRootParent();
	//if (m_pParent != nullptr)
	//{
	//	SrcPos = m_pParent->GetPosition();
	//	SrcSize = m_pParent->GetSize();
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
			NewPos.x = SrcPos.x + childSlateInfos.padding.left + accumulatePosX;
			break;
		case EHorizontalAlignment::Right:
			NewPos.x = SrcPos.x + SrcSize.x - NewSize.x - childSlateInfos.padding.right + accumulatePosX;
			break;
		case EHorizontalAlignment::Center:
			NewPos.x = SrcPos.x + (SrcSize.x / 2.f) - (NewSize.x / 2.f) + childSlateInfos.padding.left + accumulatePosX;
			break;
		default:
			NewSize.x = cellW;// -childSlateInfos.padding.left - childSlateInfos.padding.right;
			NewPos.x = SrcPos.x + childSlateInfos.padding.left + accumulatePosX;
			break;
		}
		accumulatePosX += NewSize.x + childSlateInfos.padding.left + childSlateInfos.padding.right;

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
			NewPos.y = SrcPos.y + (cellH / 2.f) - (NewSize.y / 2.f) + childSlateInfos.padding.top;
			break;
		default:
			NewSize.y = cellH - childSlateInfos.padding.top - childSlateInfos.padding.bottom;
			NewPos.y = SrcPos.y + childSlateInfos.padding.top;
			break;
		}

#if _DEBUG
		//OutputDebugStringA(std::format("size {}, {} offset {}, {}\n", NewSize.x, NewSize.y, NewPos.x, NewPos.y).c_str());
#endif
		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}
#if _DEBUG
	m_pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Red)
	);
#endif
}