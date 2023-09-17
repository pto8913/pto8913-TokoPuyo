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
	SlateContainerBase(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	SlateContainerBase(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {});
	virtual ~SlateContainerBase();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_SlateContainerBase";
		}
		return "SlateContainerBase";
	}
	virtual void Draw() override;

	virtual void AddChild(std::shared_ptr<SlateBase> in);
	virtual void RemoveChild(int idx);
	virtual void RemoveChild(std::shared_ptr<SlateBase> in);
	virtual void ClearChildren();

	virtual void UpdateWidget() override final;
protected:
	/*
		If AddChild success then this function called.
		In this function will be expect to initialized children slots properties like offset or size.
	*/
	virtual void Update() override;
public:
	virtual SlateBase* GetChildAt(int idx) const noexcept;

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
	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<SlateBase>> pChildren;
};

////////////////////////////////////////////////////////////////////////////////////////
// 
// Container only one
// 
//////////////////////////////////////////////////////////////////////////////////////// 
class SlotContainerOnlyOne : public SlateContainerBase
{
public:
	SlotContainerOnlyOne(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlateContainerBase(inSize, inD2DRT, inSlateInfos)
	{}
	SlotContainerOnlyOne(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {})
		: SlotContainerOnlyOne({ 0,0 }, inD2DRT, inSlateInfos)
	{}

	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_SlotContainerOnlyOne";
		}
		return "SlotContainerOnlyOne";
	}
	virtual void AddChild(std::shared_ptr<SlateBase> in) override final;
	virtual SlateBase* GetChildAt(int idx = 0) const noexcept override;
protected:
	virtual void Update() override;
};