#pragma once

#include "SlateContainerBase.h"

class S_Overlay : public SlateContainerBase
{
public:
	S_Overlay(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(dx, inSize, inD2DRT, inSlateInfos)
	{}
	S_Overlay(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: S_Overlay(dx, { 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual void Update() override;
	virtual void SetSize(DirectX::XMFLOAT2 inSize) override;

	virtual void AddChild(std::shared_ptr<SlateBase> in) override;
};