
#include "UI/Slate/CanvasPanel.h"

S_CanvasPanel::S_CanvasPanel(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
{
}
S_CanvasPanel::S_CanvasPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: S_CanvasPanel({0, 0}, inD2DRT, inSlateInfos)
{
}

void S_CanvasPanel::Update()
{
	const DirectX::XMFLOAT2 SrcPos = m_Position;

	DirectX::XMFLOAT2 pos;
	for (auto&& elem : m_pChildren)
	{
		pos = elem->GetPosition();
		elem->SetPosition(
			{ 
				pos.x + SrcPos.x,
				pos.y + SrcPos.y
			}
		);
	}
}