
#include "Slate/CanvasPanel.h"

S_CanvasPanel::S_CanvasPanel(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateContainerBase(inRt2D, inSize, inSlateInfos)
{
}
S_CanvasPanel::S_CanvasPanel(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos)
	: S_CanvasPanel(inRt2D, { 0,0 }, inSlateInfos)
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