#pragma once

#include "UI/UserWidget.h"

class S_TextBlock;

class SettingsUI : public UserWidget
{
public:
	SettingsUI(Object* inOwner, ID2D1HwndRenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~SettingsUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void NativeOnInitialized() override;
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "SettingsUI" + "#"s + std::to_string(mID);
	}
	void OnClickedReturnTitle(DX::MouseEvent inMouseEvent);
	void OnClickedSave(DX::MouseEvent inMouseEvent);

	void OnAudioVolumeChanged(float inValue);

	DX::FOnMouseAction OnClickedReturnToTitle;
protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	float currentAudioValue = 0.f;
	std::shared_ptr<S_TextBlock> pText_AudioVolume = nullptr;
};