#pragma once

#include "SlateContainerBase.h"

class S_Overlay : public SlateContainerBase
{
public:
	S_Overlay(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	S_Overlay(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});

	virtual void SetSize(DirectX::XMFLOAT2 inSize) override;

	virtual void AddChild(std::shared_ptr<SlateBase> in) override;

	virtual void Draw() override;
protected:
	virtual void Update() override;
};