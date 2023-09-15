#pragma once

#include "Input/Controller.h"
#include "Input/Keyboard.h"
#include "Input/MouseInterface.h"

#include "Object/Actor.h"

class DirectX11;

class Mouse;

class PlayerController : public Controller, public Actor, public std::enable_shared_from_this<PlayerController>
{
public:
	PlayerController(DirectX11& dx);
	virtual ~PlayerController();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(PlayerController).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
	virtual void Tick(DirectX11& dx, float deltaTime) override;

protected:
	// -----------------------------------
	// Main : Input
	// -----------------------------------
	virtual void InputUpdate(float deltaTime);
public:
	virtual void SetInputMode(DX::FInputMode inMode) override;

	// -----------------------------------
	// Main : Mouse
	// -----------------------------------
	LONG GetMouseDeltaX() const noexcept;
	LONG GetMouseDeltaY() const noexcept;

	void UpdateDirectInputMouse();

	// -----------------------------------
	// Main : Util
	// -----------------------------------
	DX::IMouseInterface* GetMouse();

protected:
	// -----------------------------------
	// 
	// -----------------------------------
	virtual void ExecuteTasks(DirectX11& dx) override final;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	DX::FInputMode mInputMode;

	// -----------------------------------
	// State : Mouse
	// -----------------------------------
	std::shared_ptr<Mouse> pMouse = nullptr;

	// -----------------------------------
	// State : Mouse Direct Input
	// -----------------------------------
	IDirectInputDevice8* mDirectMouse;
	DIMOUSESTATE mMouseCurrState;
	DIMOUSESTATE mMouseLastState;

	// -----------------------------------
	// State : Keyboard
	// -----------------------------------
	Keyboard mKeyBoard;
};