
#include "UI/Slate/Button.h"

#include "Helper/ColorHelper.h"

#if _DEBUG
#include <format>
#endif

S_Button::S_Button(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateButtonAppearance inButtonAppearance)
	: SlotContainerOnlyOne(inSize, inD2DRT, inSlateInfos), ButtonAppearance(inButtonAppearance)
{
	mSlateInputEventReceiveType = ESlateInputEventReceiveType::Enable;

	m_pD2DRenderTarget->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(ButtonAppearance.DefaultColor),
		&m_pBrush
	);
}
S_Button::S_Button(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateButtonAppearance inButtonAppearance)
	: S_Button({ 0,0 }, inD2DRT, inSlateInfos, inButtonAppearance)
{}

S_Button::~S_Button()
{
	OnClicked.ClearBind();
}

void S_Button::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	m_pD2DRenderTarget->FillRectangle(
		GetRect(),
		m_pBrush
	);
	SlotContainerOnlyOne::Draw();
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
		OutputDebugStringA(std::format("{} {}\n", inMouseEvent.x, inMouseEvent.y).c_str());
		OutputDebugStringA(std::format("{} {} {} {}\n", GetRect().left, GetRect().top, GetRect().right, GetRect().bottom).c_str());
#endif
		m_pBrush->SetColor(ColorHelper::ConvertColorToD2D(ButtonAppearance.PressColor));
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
			m_pBrush->SetColor(ColorHelper::ConvertColorToD2D(ButtonAppearance.HoverColor));
		}
		else
		{
			m_pBrush->SetColor(ColorHelper::ConvertColorToD2D(ButtonAppearance.DefaultColor));
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
		OutputDebugStringA("OnMouseEnter from button\n");
#endif
		m_pBrush->SetColor(ColorHelper::ConvertColorToD2D(ButtonAppearance.HoverColor));
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
		OutputDebugStringA("OnMouseLeave from button\n");
#endif
		m_pBrush->SetColor(ColorHelper::ConvertColorToD2D(ButtonAppearance.DefaultColor));
		return true;
	}
	return Out;
}
