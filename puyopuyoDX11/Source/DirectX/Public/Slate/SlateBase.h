#pragma once

#include "DirectX/DirectXHead.h"
#include "Core/DirectX.h"

class DirectX11;

enum class EHorizontalAlignment
{
	Center,
	Left,
	Right,
	Fill,
};
enum class EVerticalAlignment
{
	Center,
	Top,
	Bottom,
	Fill,
};

enum class EJustification
{
	Center,
	Left,
	Right,
};

struct FSlateInfos
{
public:
	FSlateInfos()
		: HAlign(EHorizontalAlignment::Fill),
		VAlign(EVerticalAlignment::Fill),
		padding(D2D1::RectF(0, 0, 0, 0)),
		margin(D2D1::RectF(0, 0, 0, 0))
	{};

	D2D1_RECT_F padding;
	D2D1_RECT_F margin;

	EHorizontalAlignment HAlign;
	EVerticalAlignment VAlign;
};

class SlateBase
{
public:
	SlateBase(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos());

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual void Draw() = 0;
	/*
		If AddChild success then this function called.
		In this function will be expect to initialized children slots properties like offset or size.
	*/
	virtual void Update() {};

	SlateBase* GetParent() const noexcept;
	void SetParent(SlateBase* in);
	SlateBase* GetRootParent() noexcept;

	// ------------------------------------------------
	// Main : SlateInfos
	// ------------------------------------------------
	FSlateInfos GetSlateInfos() const noexcept;
	void SetSlateInfos(const FSlateInfos& in);

	virtual void SetHorizontalAlignment(EHorizontalAlignment in);
	virtual void SetVerticalAlignment(EVerticalAlignment in);

	// ------------------------------------------------
	// Main : Transform
	// ------------------------------------------------
	D2D1_RECT_F GetRect() const noexcept;
	void SetRect(DirectX::XMFLOAT2 inSize, DirectX::XMFLOAT2 inOffset);

	bool InRect(float x, float y) const noexcept;
	
	DirectX::XMFLOAT2 GetSize() const noexcept;
	float GetWidth() const noexcept;
	float GetHeight() const noexcept;
	virtual void SetSize(DirectX::XMFLOAT2 inSize);
	
	DirectX::XMFLOAT2 GetPosition() const noexcept;
	void SetPosition(DirectX::XMFLOAT2 inPosition);

	void SetOffset(DirectX::XMFLOAT2 in);
	DirectX::XMFLOAT2 GetOffset() const noexcept;

protected:
	SlateBase* m_pParent = nullptr;

	ID2D1DeviceContext* m_pContext = nullptr;
	ID2D1RenderTarget* m_pD2DRenderTarget = nullptr;

	ID2D1SolidColorBrush* m_pBrush = nullptr;

	FSlateInfos m_SlateInfos;

	DirectX::XMFLOAT2 m_Size = { 0.f, 0.f };
	/*
		If this slate is root use m_Position.
	*/
	DirectX::XMFLOAT2 m_Position = { 0.f, 0.f };

	DirectX::XMFLOAT2 m_Offset = { 0.f, 0.f };

	D2D1_RECT_F m_Rect;
};