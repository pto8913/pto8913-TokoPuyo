#pragma once

#include "DirectX/DirectXHead.h"
#include "Core/DirectX.h"

#include "UI/Slate/SlateInterface.h"

#include "Engine/Vector.h"
#include "Engine/Rect.h"

class DirectX11;

enum class ESlateInputEventReceiveType
{
	Enable, /* Enable all event. */
	NotChildren, /* Ignore this slate, but children is not ignore. Containers Default */
	NotAll, /* Ignore all */
};

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
		padding(FRect(0, 0, 0, 0))
	{};

	FRect padding;

	EHorizontalAlignment HAlign;
	EVerticalAlignment VAlign;
};

class SlateBase : public DX::ISlateInterface
{
public:
	SlateBase(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = FSlateInfos());
	
	virtual ~SlateBase() = 0;

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual void Draw() = 0;
	void SetVisibility(bool in);
	bool GetVisibility() const noexcept;

	/* 
		Must be call this when the widget layout changed.
		ex. AddChild
	*/
	virtual void UpdateWidget();
protected:
	/*
		If AddChild success then this function called.
		In this function will be expect to initialized children slots properties like offset or size.
	*/
	virtual void Update() {};
public:
	SlateBase* GetParent() const noexcept;
	void SetParent(SlateBase* in);
	SlateBase* GetRootParent() noexcept;

	// ------------------------------------------------
	// Main : InputEvent
	// ------------------------------------------------
	virtual bool OnMouseMove(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonDown(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonHeld(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseButtonUp(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseEnter(DX::MouseEvent inMouseEvent) override;
	virtual bool OnMouseLeave(DX::MouseEvent inMouseEvent) override;
	virtual bool OnKeyDown(DX::MouseEvent inMouseEvent) override;
	virtual bool OnKeyUp(DX::MouseEvent inMouseEvent) override;

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
	FRect GetRect() const noexcept;

	template<typename T>
	bool InRect(T x, T y) const noexcept
	{
		return InRect((float)x, (float)y);
	}
	bool InRect(float x, float y) const noexcept;
	
	FVector2D GetSize() const noexcept;
	float GetWidth() const noexcept;
	float GetHeight() const noexcept;
	virtual void SetSize(FVector2D inSize);
	
	FVector2D GetPosition() const noexcept;
	void SetPosition(FVector2D inPosition);
	void AddPosition(FVector2D inPosition);

	void SetOffset(FVector2D in);
	FVector2D GetOffset() const noexcept;

protected:
	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	SlateBase* pParent = nullptr;
	ID2D1RenderTarget* pD2DRT = nullptr;
	ID2D1SolidColorBrush* pBrush = nullptr;

	// ------------------------------------------------
	// State : InputEvent
	// ------------------------------------------------
	ESlateInputEventReceiveType mSlateInputEventReceiveType;

	// ------------------------------------------------
	// State : SlateInfos
	// ------------------------------------------------
	FSlateInfos mSlateInfos;
	bool bIsVisible = true;

	FVector2D mSize;
	/*
		If this slate is root use m_Position.
	*/
	FVector2D mPosition;
	FVector2D mOffset;

	bool bIsLastInRect = false;
};