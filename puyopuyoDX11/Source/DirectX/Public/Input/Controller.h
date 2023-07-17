#pragma once

#include "Input/ControllerInterface.h"
#include <memory>

class Mouse;
class Keyboard;
class Camera;


class Controller : public DX::IControllerInterface
{
public:
	Controller();
	~Controller();

	// ---------------------------------------------------------------------------------
	// Main
	// ---------------------------------------------------------------------------------
	virtual void SetInputMode(DX::FInputMode inMode) override;

	// -----------------------------------
	// Main : Camera
	// -----------------------------------
	virtual void SetCameraEnabled(bool inEnabled) override;
	virtual bool IsEnableCamera() const noexcept override;

	// -----------------------------------
	// Main : Mouse
	// -----------------------------------
	virtual DX::IMouseInterface* GetMouseInterface() override;

	virtual void SetMouseEnabled(bool inEnabled) override;
	virtual bool IsEnableMouse() const noexcept override;
private:
	// ---------------------------------------------------------------------------------
	// State
	// ---------------------------------------------------------------------------------
	int mId = 0;
	DX::FInputMode mInputMode;

	// -----------------------------------
	// State : Camera
	// -----------------------------------
	//std::shared_ptr<Camera> m_pCamera;
	bool bEnableCamera = true;

	// -----------------------------------
	// State : Mouse
	// -----------------------------------
	std::shared_ptr<Mouse> pMouse;
};