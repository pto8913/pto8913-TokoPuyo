#pragma once

#include "UI/UserWidget.h"

class SettingsUI : public UserWidget
{
public:
	SettingsUI(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse);
	SettingsUI(DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~SettingsUI();
};