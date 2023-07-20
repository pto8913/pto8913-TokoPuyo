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
	SlateContainerBase(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateBase(inSize, inD2DRT, inSlateInfos)
	{
		mSlateInputEventReceiveType = ESlateInputEventReceiveType::NotChildren;
	}
	SlateContainerBase(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase({0,0}, inD2DRT, inSlateInfos)
	{}

	virtual void Draw() override;

	virtual void AddChild(std::shared_ptr<SlateBase> in);
	virtual void RemoveChild(int idx);
	virtual void ClearChildren();
	/* 
		If AddChild success then this function called. 
		In this function will be expect to initialized children slots properties like offset or size.
	*/
	virtual void Update() override;
	virtual SlateBase* GetSlotAt(int idx) const noexcept;
	
	std::vector<std::shared_ptr<SlateBase>> GetChiledren() const noexcept;
	size_t GetChildrenCount() const noexcept;

	// ------------------------------------------------
	// Main : Event
	// ------------------------------------------------
	virtual bool OnMouseMove(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonDown(DX::MouseEvent) override;
	virtual bool OnMouseButtonHeld(DX::MouseEvent) override;
	virtual bool OnMouseButtonUp(DX::MouseEvent) override;
	virtual bool OnMouseEnter(DX::MouseEvent) override;
	virtual bool OnMouseLeave(DX::MouseEvent) override;
	virtual bool OnKeyDown(DX::MouseEvent) override;
	virtual bool OnKeyUp(DX::MouseEvent) override;

protected:
	std::vector<std::shared_ptr<SlateBase>> m_pChildren;
};

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container only one
// 
//////////////////////////////////////////////////////////////////////////////////////// 
class SlotContainerOnlyOne : public SlateContainerBase
{
public:
	SlotContainerOnlyOne(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
	{}
	SlotContainerOnlyOne(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlotContainerOnlyOne({ 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual void AddChild(std::shared_ptr<SlateBase> in) override final;
	virtual SlateBase* GetSlotAt(int idx = 0) const noexcept override;
};