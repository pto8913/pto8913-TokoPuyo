
#include "Slate/Button.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

#if _DEBUG
#include <format>
#endif

S_Button::S_Button(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos, FSlateButtonAppearance inAppearance)
	: SlotContainerOnlyOne(inD2DRT, inSize, inSlateInfos), mAppearance(inAppearance)
{
	mSlateInputEventReceiveType = ESlateInputEventReceiveType::Enable;

	pD2DRT->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.DefaultColor),
		&pBrush
	);
}
S_Button::S_Button(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateButtonAppearance inAppearance)
	: S_Button(inD2DRT, { 0,0 }, inSlateInfos, inAppearance)
{}

S_Button::~S_Button()
{
	OnClicked.ClearBind();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void S_Button::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	pD2DRT->FillRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
	SlotContainerOnlyOne::Draw();
}

void S_Button::SetAppearance(const FSlateButtonAppearance& in)
{
	mAppearance = in;
}
FSlateButtonAppearance& S_Button::GetAppearance()
{
	return mAppearance;
}

// ------------------------------------------------
// Main : Event
// ------------------------------------------------
bool S_Button::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	bool Out = SlotContainerOnlyOne::OnMouseButtonDown(inMouseEvent);
	if (Out)
	{
#if _DEBUG
		//OutputDebugStringA(std::format("{} {}\n", inMouseEvent.x, inMouseEvent.y).c_str());
		//OutputDebugStringA(std::format("{} {} {} {}\n", GetRect().left, GetRect().top, GetRect().right, GetRect().bottom).c_str());
#endif
		pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.PressColor));
		OnClicked.Broadcast(inMouseEvent);
		return true;
	}
	return Out;
}
bool S_Button::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	bool Out = SlotContainerOnlyOne::OnMouseButtonHeld(inMouseEvent);
	if (Out)
	{
		return true;
	}
	return Out;
}
bool S_Button::OnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	bool Out = SlotContainerOnlyOne::OnMouseButtonUp(inMouseEvent);
	if (Out)
	{
		if (InRect(inMouseEvent.x, inMouseEvent.y))
		{
			pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.HoverColor));
		}
		else
		{
			pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.DefaultColor));
		}
		return true;
	}
	return Out;
}
bool S_Button::OnMouseEnter(DX::MouseEvent inMouseEvent)
{
	bool Out = SlotContainerOnlyOne::OnMouseEnter(inMouseEvent);
	if (Out)
	{
#if _DEBUG
		//OutputDebugStringA("OnMouseEnter from button\n");
#endif
		pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.HoverColor));
		return true;
	}
	return Out;
}
bool S_Button::OnMouseLeave(DX::MouseEvent inMouseEvent)
{
	bool Out = SlotContainerOnlyOne::OnMouseLeave(inMouseEvent);
	if (Out)
	{
#if _DEBUG
		//OutputDebugStringA("OnMouseLeave from button\n");
#endif
		pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.DefaultColor));
		return true;
	}
	return Out;
}
