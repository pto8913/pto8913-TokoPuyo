
#include "UI/Slate/HorizontalBox.h"

#define _DEBUG 0

#if _DEBUG
#include <format>
#endif

S_HorizontalBox::S_HorizontalBox(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
{
}

S_HorizontalBox::S_HorizontalBox(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: S_HorizontalBox({ 0,0 }, inD2DRT, inSlateInfos)
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
#if _DEBUG
	ID2D1SolidColorBrush* brush = nullptr;
	pD2DRT->CreateSolidColorBrush(
		D2D1::ColorF(1, 0, 1, 1),
		&brush
	);
	pD2DRT->DrawRectangle(
		GetRect(), brush
	);
#endif
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

#if _DEBUG
		OutputDebugStringA(std::format("HB child size {}, {} pos {}, {}\n", NewSize.x, NewSize.y, NewPos.x, NewPos.y).c_str());
#endif
		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}
#if _DEBUG
	pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Red)
	);
#endif
}