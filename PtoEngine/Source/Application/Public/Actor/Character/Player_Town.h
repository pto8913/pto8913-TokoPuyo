#pragma once

#include "Player.h"

class Player_Town : public Player
{
public:
	Player_Town(DirectX11& dx);
	virtual ~Player_Town();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

protected:
	// ---------------------------
	// Main : Movement
	// ---------------------------
	virtual void InputUpdate() override;
	virtual void Move(const float& x, const float& y) override;
};