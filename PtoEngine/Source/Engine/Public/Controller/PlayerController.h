#pragma once

#include "Input/Controller.h"
#include "Input/Keyboard.h"
#include "Input/MouseInterface.h"

#include "Object/Actor.h"

class DirectX11;

class Mouse;

class Player;
class HUD;

class PlayerController : public Controller, public Actor
{
public:
	PlayerController(DirectX11& dx);
	virtual ~PlayerController();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
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
	std::shared_ptr<Player>& GetPlayer();
	std::shared_ptr<HUD>& GetHUD();
protected:
	// -----------------------------------
	// 
	// -----------------------------------
	virtual void ExecuteTasks(DirectX11& dx) override final;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Player> pPlayer = nullptr;
	std::shared_ptr<HUD> pHUD = nullptr;

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