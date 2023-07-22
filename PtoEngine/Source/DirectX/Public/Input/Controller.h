#pragma once

#include "Input/ControllerInterface.h"
#include "Input/Keyboard.h"
#include <memory>

class DirectX11;
class Mouse;
class Keyboard;
class Camera;

class Controller : public DX::IControllerInterface
{
public:
	Controller(DirectX11& dx, HINSTANCE hInstance, HWND hWnd);
	~Controller();

	// ---------------------------------------------------------------------------------
	// Main
	// ---------------------------------------------------------------------------------
	virtual void SetInputMode(DX::FInputMode inMode) override;
	virtual DX::FInputMode GetInputMode() const noexcept override;
	virtual void ExecuteTasks(DirectX11& dx) override;

	// -----------------------------------
	// Main : Camera
	// -----------------------------------
	virtual DX::ICameraInterface* GetCameraInterface() override;

	virtual void SetCameraEnabled(bool inEnabled) override;
	virtual bool IsEnableCamera() const noexcept override;

	// -----------------------------------
	// Main : Mouse
	// -----------------------------------
	virtual DX::IMouseInterface* GetMouseInterface() override;

	virtual void SetMouseEnabled(bool inEnabled) override;
	virtual bool IsEnableMouse() const noexcept override;

	virtual LONG GetMouseDeltaX() const noexcept override;
	virtual LONG GetMouseDeltaY() const noexcept override;

	void UpdateDirectInputMouse();

private:
	// ---------------------------------------------------------------------------------
	// State
	// ---------------------------------------------------------------------------------
	int mId = 0;
	DX::FInputMode mInputMode = DX::FInputMode::GameOnly;

	// -----------------------------------
	// State : Camera
	// -----------------------------------
	std::shared_ptr<Camera> pCamera;
	bool bEnableCamera = true;

	// -----------------------------------
	// State : Mouse
	// -----------------------------------
	std::shared_ptr<Mouse> pMouse;

	// -----------------------------------
	// State : Mouse Direct Input
	// -----------------------------------
	IDirectInputDevice8* mDirectMouse;
	DIMOUSESTATE mMouseCurrState;
	DIMOUSESTATE mMouseLastState;

	// -----------------------------------
	// State : Keyboard
	// -----------------------------------
	Keyboard m_keyBoard;
};