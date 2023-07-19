
#include "Slate/Button.h"

S_Button::S_Button(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateButtonAppearance inButtonAppearance)
	: SlotContainerOnlyOne(dx, inSize, inD2DRT, inSlateInfos), ButtonAppearance(inButtonAppearance)
{
	mSlateInputEventReceiveType = ESlateInputEventReceiveType::Enable;

	m_pD2DRenderTarget->CreateSolidColorBrush(
		ButtonAppearance.DefaultColor,
		&m_pBrush
	);
}

void S_Button::Draw()
{
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
		m_pBrush->SetColor(ButtonAppearance.PressColor);
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
		m_pBrush->SetColor(ButtonAppearance.DefaultColor);
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
		m_pBrush->SetColor(ButtonAppearance.HoverColor);
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
		m_pBrush->SetColor(ButtonAppearance.DefaultColor);
		return true;
	}
	return Out;
}
