
#include "Slate/VerticalBox.h"

S_VerticalBox::S_VerticalBox(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateContainerBase(inD2DRT, inSize, inSlateInfos)
{
#if _DEBUG
	pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Red)
	);
#endif
}

S_VerticalBox::S_VerticalBox(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: S_VerticalBox(inD2DRT, { 0,0 }, inSlateInfos)
{}

S_VerticalBox::~S_VerticalBox()
{
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void S_VerticalBox::Update()
{
	const FRect containerRect = GetRect();

	const int numOfChild = (int)pChildren.size();
	const float cellH = GetHeight() / numOfChild;
	const float cellW = GetWidth();

	FVector2D NewSize = { 0, 0 };
	FVector2D NewPos = { 0, 0 };
	float accumulatePosY = 0.f;

	FVector2D SrcPos = mPosition;
	FVector2D SrcSize = mSize;
	//const SlateBase* pRootParent = GetRootParent();
	//if (pParent != nullptr)
	//{
	//	SrcPos = pParent->GetPosition();
	//	SrcSize = pParent->GetSize();
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
			NewPos.y = SrcPos.y + childSlateInfos.padding.top + accumulatePosY;
			break;
		case EVerticalAlignment::Bottom:
			NewPos.y = SrcPos.y + SrcSize.y - NewSize.y - childSlateInfos.padding.bottom + accumulatePosY;
			break;
		case EVerticalAlignment::Center:
			NewPos.y = SrcPos.y + (cellH / 2.f) - (NewSize.y / 2.f) + childSlateInfos.padding.top + accumulatePosY;
			break;
		default:
			NewSize.y = NewSize.y;// -childSlateInfos.padding.top - childSlateInfos.padding.bottom;
			NewPos.y = SrcPos.y + childSlateInfos.padding.top + accumulatePosY;
			//accumulatePosY += childSlateInfos.padding.top + childSlateInfos.padding.bottom;
			break;
		}
		accumulatePosY += NewSize.y + childSlateInfos.padding.top + childSlateInfos.padding.bottom;

		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}

}