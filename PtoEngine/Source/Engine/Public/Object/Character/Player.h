#pragma once

#include "Object/Character/CharacterBase.h"

class HUD;

class Player : public CharacterBase
{
public:
	Player(DirectX11& dx);

	virtual void TurnElapsed() override {};

	virtual void Tick(DirectX11& dx, float deltaTime) override;
protected:
	void InputUpdate();
};