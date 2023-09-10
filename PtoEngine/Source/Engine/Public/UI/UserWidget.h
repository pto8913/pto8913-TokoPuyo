#pragma once

#include "Object/Object.h"
#include "Object/Widget/WidgetBase.h"

#include "Input/MouseInterface.h"

#include "Slate/SlateContainerBase.h"
#include "UI/WidgetAnimation.h"

class DirectX11;

class WidgetAnimation;

class UserWidget : public WidgetBase, public Object
{
public:
	UserWidget(DirectX11& dx, DX::IMouseInterface* mouse, float windowSizeW, float windowSizeH);
	UserWidget(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse, float windowSizeW, float windowSizeH);
	virtual ~UserWidget();
protected:
	virtual void Draw() override;

public:

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void AddToViewport(double inZOrder = 0);
	void RemoveFromParent();

	bool IsInViewport() const noexcept;
	double GetZOrder() const noexcept;

	virtual World* GetWorld() override;

	ID2D1RenderTarget* GetRt2D();

	virtual void AddSlate(std::shared_ptr<SlateBase> inSlate);
	virtual void RemoveSlate(std::shared_ptr<SlateBase> inSlate);

protected:
	// --------------------------
	// Main : Slate
	// --------------------------
	template<typename TClass, typename ...Args>
	std::shared_ptr<TClass> MakeSlate(Args&& ...args)
	{
		return std::make_shared<TClass>(GetRt2D(), std::forward<Args>(args)...);
	}

	// --------------------------
	// Main : Animation
	// --------------------------
	void AddAnimation(WidgetAnimation in);

	// --------------------------
	// Main : Input
	// --------------------------
	void OnMouseMove(DX::MouseEvent);
	void OnMouseButtonDown(DX::MouseEvent);
	void OnMouseButtonHeld(DX::MouseEvent);
	void OnMouseButtonUp(DX::MouseEvent);
	void OnMouseEnter(DX::MouseEvent);
	void OnMouseLeave(DX::MouseEvent);
	void OnKeyDown(DX::MouseEvent);
	void OnKeyUp(DX::MouseEvent);

	virtual bool NativeOnMouseMove(DX::MouseEvent);
	virtual bool NativeOnMouseButtonDown(DX::MouseEvent);
	virtual bool NativeOnMouseButtonHeld(DX::MouseEvent);
	virtual bool NativeOnMouseButtonUp(DX::MouseEvent);
	virtual bool NativeOnMouseEnter(DX::MouseEvent);
	virtual bool NativeOnMouseLeave(DX::MouseEvent);
	virtual bool NativeOnKeyDown(DX::MouseEvent);
	virtual bool NativeOnKeyUp(DX::MouseEvent);
public:
	// --------------------------
	// 
	// --------------------------
	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override final;

private:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

	/* The higher the number, the more it appears in the front. */
	double ZOrder = 1.0;
protected:
	bool bIsInViewport = false;

	std::shared_ptr<SlateContainerBase> pRootSlate = nullptr;
	DX::IMouseInterface* pMouse;
	Object* pOwner = nullptr;

	TArray<WidgetAnimation> mAnimations;

};