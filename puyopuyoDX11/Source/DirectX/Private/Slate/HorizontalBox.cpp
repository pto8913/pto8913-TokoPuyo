
#include "Slate/HorizontalBox.h"

S_HorizontalBox::S_HorizontalBox(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateContainerBase(dx, inSize, inD2DRT, inSlateInfos)
{

}
void S_HorizontalBox::Update()
{
	const D2D1_RECT_F containerRect = GetRect();

	const int numOfChild = (int)m_pChildren.size();
	const float cellW = GetWidth() / numOfChild;
	const float halfCellW = cellW / 2;
	const float halfCellH = GetHeight() / 2;

	D2D1_RECT_F childRect_New;
	for (int i = 0; i < numOfChild; ++i)
	{
		//SlateBase*& pChild = m_pChildren[i];

		//D2D1_RECT_F childRect = pChild->GetRect();
		//const float childWidth = pChild->GetWidth();
		//const float halfChildWidth = childWidth / 2;

		//const float childHeight = pChild->GetHeight();
		//const float halfChildHeight = childHeight / 2;

		//float left = containerRect.left + cellW * i;
		//switch (m_ChildInfos.HAlign)
		//{
		//case EHorizontalAlignment::Left:
		//	childRect_New.left = left;
		//	if (childRect_New.right > cellW)
		//	{
		//		childRect_New.right = cellW;
		//	}
		//	break;
		//case EHorizontalAlignment::Right:
		//	childRect_New.right = cellW;
		//	if (childRect_New.left < left)
		//	{
		//		childRect_New.left = left;
		//	}
		//	break;
		//default:
		//	childRect_New.left = halfCellW - halfChildWidth;
		//	childRect_New.right = halfCellW + halfChildWidth;
		//	break;
		//}
		//switch (m_ChildInfos.VAlign)
		//{
		//case EVerticalAlignment::Top:
		//	childRect_New.top = containerRect.top;
		//	if (childRect_New.bottom > containerRect.bottom)
		//	{
		//		childRect_New.bottom = containerRect.bottom;
		//	}
		//	break;
		//case EVerticalAlignment::Bottom:
		//	childRect_New.bottom = containerRect.bottom;
		//	if (childRect_New.top < containerRect.top)
		//	{
		//		childRect_New.top = containerRect.top;
		//	}
		//	break;
		//default:
		//	childRect_New.top = halfCellH - halfChildHeight;
		//	childRect_New.bottom = halfCellH + halfChildHeight;
		//	break;
		//}

		//pChild->SetRect(childRect_New);
	}
}