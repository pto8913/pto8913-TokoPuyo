#pragma once

#include "SlateBase.h"

class DirectX11;


////////////////////////////////////////////////////////////////////////////////////////
// 
// Container
// 
//////////////////////////////////////////////////////////////////////////////////////// 
class SlateContainerBase : public SlateBase
{
public:
	SlateContainerBase(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateBase(dx, inSize, inD2DRT, inSlateInfos)
	{}
	SlateContainerBase(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(dx, {0,0}, inD2DRT, inSlateInfos)
	{}

	virtual void Draw() override;

	virtual void AddChild(SlateBase* in);
	/* 
		If AddChild success then this function called. 
		In this function will be expect to initialized children slots properties like offset or size.
	*/
	virtual void Update() override {};
	SlateBase* GetSlotAt(int idx) const noexcept;
	
	std::vector<SlateBase*> GetChiledren() const noexcept;
	size_t GetChildrenCount() const noexcept;
protected:
	std::vector<SlateBase*> m_pChildren;
};

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container only one
// 
//////////////////////////////////////////////////////////////////////////////////////// 
class SlotContainerOnlyOne : public SlateContainerBase
{
public:
	SlotContainerOnlyOne(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(dx, inSize, inD2DRT, inSlateInfos)
	{}
	SlotContainerOnlyOne(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlotContainerOnlyOne(dx, { 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual void AddChild(SlateBase* in) override final;
	SlateBase* GetChild() const noexcept;
};