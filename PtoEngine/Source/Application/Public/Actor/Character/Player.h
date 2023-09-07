#pragma once

#include "Actor/Character/CharacterBase.h"
#include "Actor/Character/PlayerTypes.h"

#include "Input/Keyboard.h"

class HUD;
class Level2D;
class PlayerController;

class Player : public CharacterBase
{
public:
	Player(DirectX11& dx);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;

	virtual void TurnElapsed() override {};

	virtual void Tick(DirectX11& dx, float deltaTime) override;

	// ---------------------------
	// Main : Movement
	// ---------------------------
	void SetEnableInput(bool in);
	bool IsEnableInput() const noexcept;
protected:
	virtual void InputUpdate();
	virtual void Move(const float& x, const float& y);
public:

	// ---------------------------
	// Main : Delegate
	// ---------------------------

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	bool bIsEnableInput = true;
	bool bCanMove = true;

	// ---------------------------
	// State : Input
	// ---------------------------
	Keyboard::InputAction InputW;
	Keyboard::InputAxis InputAxisW;
	Keyboard::InputAction InputA;
	Keyboard::InputAxis InputAxisA;
	Keyboard::InputAction InputS;
	Keyboard::InputAxis InputAxisS;
	Keyboard::InputAction InputD;
	Keyboard::InputAxis InputAxisD;
	Keyboard::InputAxis InputAxisLShift;
	Keyboard::InputAction InputQ;
	Keyboard::InputAction InputE;
};