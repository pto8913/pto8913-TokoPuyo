
#include "Slate/HorizontalBox.h"

S_HorizontalBox::S_HorizontalBox(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateContainerBase(inRt2D, inSize, inSlateInfos)
{
#if _DEBUG
	pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Red)
	);
#endif
}

S_HorizontalBox::S_HorizontalBox(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos)
	: S_HorizontalBox(inRt2D, { 0,0 }, inSlateInfos)
{
}
S_HorizontalBox::~S_HorizontalBox()
{
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void S_HorizontalBox::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	SlateContainerBase::Draw();
}

void S_HorizontalBox::Update()
{
	const FRect containerRect = GetRect();

	const int numOfChild = (int)pChildren.size();
	const float cellH = GetHeight();
	const float cellW = GetWidth() / numOfChild;

	FVector2D NewSize = { 0, 0 };
	FVector2D NewPos = { 0, 0 };
	float accumulatePosX = 0.f;

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
			NewPos.x = SrcPos.x + childSlateInfos.padding.left;
			break;
		}
		NewPos.x += accumulatePosX;
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
			NewPos.y = SrcPos.y + childSlateInfos.padding.top;
			break;
		}

		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}
}