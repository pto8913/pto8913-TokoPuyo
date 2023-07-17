#pragma once

#include "Object/DrawableObject2D.h"
#include "Render/Factory/TransformConstantBuffer.h"

#include "Object/Widget/WidgetInterface.h"

class DirectX11;

class ScreenTextOnlyOutput;

class WidgetBase : private DrawableObject2D, public DX::IWidgetInterface
{
public:
	WidgetBase(DirectX11& dx, UINT windowSizeW, UINT windowSizeH, int inZOrder = 0);

	virtual void ExecuteTasks(DirectX11& dx) override final;

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override final;

	void AddToViewport();
	bool IsInViewport() const noexcept;
	int GetZOrder() const noexcept;

	virtual void SetVisibility(bool inVisible) override;
	virtual bool GetVisibility() override;

	// ----------------------------------------------------------
	// Main : Event
	// ----------------------------------------------------------
	virtual bool OnMouseButtonDown(DX::MouseEvent) override { return false; };
	virtual bool OnMouseButtonHeld(DX::MouseEvent) override { return false; };
	virtual bool OnMouseButtonUp(DX::MouseEvent) override { return false; };
	virtual bool OnMouseEnter(DX::MouseEvent) override { return false; };
	virtual bool OnMouseLeave(DX::MouseEvent) override { return false; };
	virtual bool OnKeyDown(DX::MouseEvent) override { return false; };
	virtual bool OnKeyUp(DX::MouseEvent) override { return false; };
protected:
	virtual void Draw() = 0;
	void DrawInternal();
private:
	std::shared_ptr<TransformConstantBuffer> m_pTCB = nullptr;
	std::shared_ptr<ScreenTextOnlyOutput> m_pScreenTextOnlyOutput = nullptr;

	/* The higher the number, the more it appears in the front. */
	double ZOrder = 1.0;
protected:
	const TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};

	ID2D1RenderTarget* m_pRt2D = nullptr;
	IDXGIKeyedMutex* m_pMutex11 = nullptr;
	IDXGIKeyedMutex* m_pMutex10 = nullptr;
	D2D1_COLOR_F m_ClearColor;

	bool bIsInViewport = false;
};