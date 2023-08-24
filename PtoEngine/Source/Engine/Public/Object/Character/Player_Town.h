#pragma once

#include "Player.h"

class Player_Town : public Player
{
public:
	Player_Town(DirectX11& dx);
	virtual ~Player_Town();

	// ---------------------------
	// Main : Transform
	// ---------------------------
	virtual void SetActorLocation(const FVector& in) override;

protected:
	// ---------------------------
	// Main : Movement
	// ---------------------------
	virtual void InputUpdate() override;
	virtual void Move(const int& x, const int& y) override;
};