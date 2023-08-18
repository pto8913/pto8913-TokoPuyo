#pragma once

#include "Object/Object.h"
#include "Object/Widget/WidgetBase.h"

#include "Input/MouseInterface.h"

#include "UI/Slate/SlateContainerBase.h"
#include "UI/WidgetAnimation.h"

class DirectX11;

class WidgetAnimation;

class UserWidget : public WidgetBase, public Object
{
public:
	UserWidget(DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH);
	virtual ~UserWidget();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void AddToViewport(double inZOrder = 0);
	bool IsInViewport() const noexcept;
	double GetZOrder() const noexcept;

protected:
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
	virtual void ExecuteTasks(DirectX11& dx) override final;
	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override final;

private:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

	/* The higher the number, the more it appears in the front. */
	double ZOrder = 1.0;
protected:
	bool bIsInViewport = false;

	std::shared_ptr<SlateContainerBase> m_pRootSlate = nullptr;
	DX::IMouseInterface* pMouse;

	TArray<WidgetAnimation> mAnimations;
};