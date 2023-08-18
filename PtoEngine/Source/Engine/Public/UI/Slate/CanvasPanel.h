#pragma once

#include "SlateContainerBase.h"

class S_CanvasPanel : public SlateContainerBase
{
public:
	S_CanvasPanel(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	S_CanvasPanel(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});

	virtual void Update() override;
};