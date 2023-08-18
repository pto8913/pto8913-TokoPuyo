
#include "UI/Slate/GridPanel.h"

#if _DEBUG
#include <format>
#endif

S_GridPanel::S_GridPanel(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateGridPanelAppearance inAppearance)
	: SlateContainerBase(inSize, inD2DRT, inSlateInfos),
	appearance(inAppearance)
{
	m_pD2DRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(1, 1, 0),
		&m_pBrush
	);
}
S_GridPanel::S_GridPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateGridPanelAppearance inAppearance)
	: S_GridPanel({ 0,0 }, inD2DRT, inSlateInfos, inAppearance)
{
}

std::shared_ptr<SlateBase>& S_GridPanel::GetChildAt(const int& x, const int& y)
{
	return m_pChildren[y * appearance.column + x];
}
void S_GridPanel::SetAppearance(const FSlateGridPanelAppearance& in)
{
	appearance = in;
}

void S_GridPanel::Update()
{
	const D2D1_RECT_F containerRect = GetRect();

	const int numOfChild = (int)m_pChildren.size();
	const float cellH = GetHeight() / numOfChild;
	const float cellW = GetWidth() / numOfChild;

	DirectX::XMFLOAT2 NewSize = { 0, 0 };
	DirectX::XMFLOAT2 NewPos = { 0, 0 };
	float accumulatePosX = 0.f, accumulatePosY = 0.f;

	DirectX::XMFLOAT2 SrcPos = { GetRect().left, GetRect().top };
	DirectX::XMFLOAT2 SrcSize = m_Size;
	//const SlateBase* pRootParent = GetRootParent();
	//if (m_pParent != nullptr)
	//{
	//	SrcPos = m_pParent->GetPosition();
	//	SrcSize = m_pParent->GetSize();
	//}

	float maxSizeY = 0, maxPadT = 0, maxPadB = 0;
	for (int y = 0; y < appearance.row; ++y)
	{
		for (int x = 0; x < appearance.column; ++x)
		{
			int idx = y * appearance.column + x;
			if (m_pChildren.size() > idx)
			{
				auto&& pChild = m_pChildren[idx];
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
#if _DEBUG
	m_pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Red)
	);
#endif
}
