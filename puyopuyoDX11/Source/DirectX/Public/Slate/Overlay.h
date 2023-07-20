#pragma once

#include "SlateContainerBase.h"

class S_Overlay : public SlateContainerBase
{
public:
	S_Overlay(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
	{}
	S_Overlay(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: S_Overlay({ 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual void Update() override;
	virtual void SetSize(DirectX::XMFLOAT2 inSize) override;

	virtual void AddChild(std::shared_ptr<SlateBase> in) override;
};