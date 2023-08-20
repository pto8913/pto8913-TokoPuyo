#pragma once

#include "Object/Character/CharacterBase.h"

class HUD;

class Player : public CharacterBase
{
public:
	Player(DirectX11& dx);

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------

	virtual void TurnElapsed() override {};

	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void SetEnableInput(bool in);
	bool IsEnableInput() const noexcept;
protected:
	void InputUpdate();

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	bool bIsEnableInput = true;
};