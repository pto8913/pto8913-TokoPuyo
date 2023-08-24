#pragma once

#include "Player.h"

class Player_Dungeon : public Player
{
public:
	Player_Dungeon(DirectX11& dx);
	virtual ~Player_Dungeon();

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
public:
	void EnterDungeonBlock(const FVector2D& in);
};