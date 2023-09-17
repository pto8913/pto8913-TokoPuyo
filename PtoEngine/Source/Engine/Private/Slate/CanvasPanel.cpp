
#include "Slate/CanvasPanel.h"

S_CanvasPanel::S_CanvasPanel(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateContainerBase(inD2DRT, inSize, inSlateInfos)
{
}
S_CanvasPanel::S_CanvasPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: S_CanvasPanel(inD2DRT, { 0,0 }, inSlateInfos)
{
}
S_CanvasPanel::~S_CanvasPanel()
{
}

void S_CanvasPanel::Update()
{
	const FVector2D SrcPos = mPosition;

	FVector2D pos;
	for (auto&& elem : pChildren)
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