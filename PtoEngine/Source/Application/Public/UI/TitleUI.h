#pragma once

#include "UI/UserWidget.h"

class S_TextBlock;
class S_Border;
class S_Image;
class S_Button;

class TitleUI : public UserWidget
{
public:
	TitleUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~TitleUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void NativeOnInitialized() override;
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "TitleUI" + "#"s + std::to_string(mID);
	}
protected:
	void OnClickedTokoPuyoButton(DX::MouseEvent inMouseEvent);
	void OnClickedVSCPU(DX::MouseEvent inMouseEvent);
	void OnClickedSettings(DX::MouseEvent inMouseEvent);
	void OnClickedExit(DX::MouseEvent inMouseEvent);
public:
	DX::FOnMouseAction OnClickedTokoPuyo;
	DX::FOnMouseAction OnClickedOpenSettings;
protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
};