#pragma once

#include "UI/UserWidget.h"

class S_TextBlock;
class S_Border;
class S_Image;
class S_Button;

class TitleUI : public UserWidget
{
public:
	TitleUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse);
	TitleUI(DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~TitleUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
protected:
	void OnClickedTokoPuyoButton(DX::MouseEvent inMouseEvent);
	void OnClickedVSCPU(DX::MouseEvent inMouseEvent);
	void OnClickedExit(DX::MouseEvent inMouseEvent);
public:
	DX::FOnMouseAction OnClickedTokoPuyo;
protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

};