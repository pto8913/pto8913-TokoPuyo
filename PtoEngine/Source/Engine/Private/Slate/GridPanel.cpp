
#include "Slate/GridPanel.h"

S_GridPanel::S_GridPanel(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos, FSlateGridPanelAppearance inAppearance)
	: SlateContainerBase(inD2DRT, inSize, inSlateInfos),
	mAppearance(inAppearance)
{
#if _DEBUG
	pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Red)
	);
#endif
}
S_GridPanel::S_GridPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateGridPanelAppearance inAppearance)
	: S_GridPanel(inD2DRT, { 0,0 }, inSlateInfos, inAppearance)
{
}
S_GridPanel::~S_GridPanel()
{
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
SlateBase* S_GridPanel::GetChildAt(const int& x, const int& y)
{
	return pChildren[y * mAppearance.column + x].get();
}
void S_GridPanel::SetAppearance(const FSlateGridPanelAppearance& in)
{
	mAppearance = in;
}
FSlateGridPanelAppearance& S_GridPanel::GetAppearance()
{
	return mAppearance;
}

void S_GridPanel::Update()
{
	const FRect containerRect = GetRect();

	const int numOfChild = (int)pChildren.size();
	const float cellH = GetHeight() / numOfChild;
	const float cellW = GetWidth() / numOfChild;

	FVector2D NewSize = { 0, 0 };
	FVector2D NewPos = { 0, 0 };
	float accumulatePosX = 0.f, accumulatePosY = 0.f;

	FVector2D SrcPos = { GetRect().left, GetRect().top };
	FVector2D SrcSize = mSize;
	//const SlateBase* pRootParent = GetRootParent();
	//if (pParent != nullptr)
	//{
	//	SrcPos = pParent->GetPosition();
	//	SrcSize = pParent->GetSize();
	//}

	float maxSizeY = 0, maxPadT = 0, maxPadB = 0;
	for (int y = 0; y < mAppearance.row; ++y)
	{
		for (int x = 0; x < mAppearance.column; ++x)
		{
			int idx = y * mAppearance.column + x;
			if (pChildren.size() > idx)
			{
				auto&& pChild = pChildren[idx];
				const FSlateInfos childSlateInfos = pChild->GetSlateInfos();
				const float childWidth = pChild->GetWidth();
				const float childHeight = pChild->GetHeight();
				NewSize.x = cellW;
				NewSize.y = cellH;

				if (childWidth > 0)
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

				if (childHeight > 0)
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
				NewPos.y += accumulatePosY;

				pChild->SetSize(NewSize);
				pChild->SetPosition(NewPos);
				pChild->Draw();

				//OutputDebugStringA(std::format("GPChild size {}, {} pos {}, {}\n", NewSize.x, NewSize.y, NewPos.x, NewPos.y).c_str());

				if (maxSizeY < NewSize.y)
				{
					maxSizeY = NewSize.y;
					maxPadT = childSlateInfos.padding.top;
					maxPadB = childSlateInfos.padding.bottom;
				}
			}
		}
		accumulatePosX = 0;
		accumulatePosY += maxSizeY + maxPadT + maxPadB;
	}
}
