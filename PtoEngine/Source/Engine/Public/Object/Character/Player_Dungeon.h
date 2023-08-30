#pragma once

#include "Player.h"

class Player_Dungeon : public Player
{
public:
	Player_Dungeon(DirectX11& dx);
	virtual ~Player_Dungeon();

protected:
	// ---------------------------
	// Main : Movement
	// ---------------------------
	virtual void InputUpdate() override;
	virtual void Move(const float& x, const float& y) override;
public:
	void EnterDungeonBlock(const FVector2D& in);
};