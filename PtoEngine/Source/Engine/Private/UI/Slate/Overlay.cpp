
#include "UI/Slate/Overlay.h"

#define _DEBUG 0

#if _DEBUG
#include <format>
#endif


S_Overlay::S_Overlay(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
{
}
S_Overlay::S_Overlay(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: S_Overlay({ 0,0 }, inD2DRT, inSlateInfos)
{
}
S_Overlay::~S_Overlay()
{
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void S_Overlay::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	SlateContainerBase::Draw();
#if _DEBUG

	ID2D1SolidColorBrush* brush = nullptr;
	pD2DRT->CreateSolidColorBrush(
		D2D1::ColorF(1, 0, 0, 1),
		&brush
	);
	pD2DRT->DrawRectangle(
		GetRect(), brush
	);
#endif
}

void S_Overlay::AddChild(std::shared_ptr<SlateBase> in)
{
	SetSize({ in->GetWidth(), in->GetHeight() });

	SlateContainerBase::AddChild(in);
}

void S_Overlay::SetSize(FVector2D inSize)
{
	mSize.x = max(inSize.x, mSize.x);
	mSize.y = max(inSize.y, mSize.y);
}

void S_Overlay::Update()
{
	const int numOfChild = (int)pChildren.size();
	const float cellH = GetHeight();
	const float cellW = GetWidth();

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
		const FSlateInfos& childSlateInfos = pChild->GetSlateInfos();
		const float childWidth = pChild->GetWidth();
		const float childHeight = pChild->GetHeight();

		const auto& childPos = pChild->GetPosition();
		NewPos = childPos;

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
			NewPos.y = SrcPos.y + childSlateInfos.padding.top;
			break;
		}
#if _DEBUG
		OutputDebugStringA(std::format("Overlay child size {}, {} pos {}, {}\n", NewSize.x, NewSize.y, NewPos.x, NewPos.y).c_str());
#endif
		pChild->SetSize(NewSize);
		pChild->SetPosition(NewPos);
		pChild->Draw();
	}
#if _DEBUG
	pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Yellow)
	);
#endif
}