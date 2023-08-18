#pragma once

#include "SlateContainerBase.h"

class SlateSlotBase;

class S_HorizontalBox : public SlateContainerBase
{
public:
	S_HorizontalBox(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});		
	S_HorizontalBox(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	
	virtual void Draw() override;
protected:
	virtual void Update() override;
};