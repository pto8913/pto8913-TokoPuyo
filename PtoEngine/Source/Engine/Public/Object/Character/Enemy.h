#pragma once

#include "Object/Character/CharacterBase.h"

class Enemy : public CharacterBase
{
public:
	Enemy(DirectX11& dx, const ECharacterId& CharacterType, const UINT16& inX, const UINT16& inY);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

	virtual void TurnElapsed() override {};

	DirectX::XMFLOAT2 GetCoord();

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	UINT16 x, y;
};