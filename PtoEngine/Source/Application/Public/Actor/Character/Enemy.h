#pragma once

#include "Actor/Character/CharacterBase.h"

class Enemy : public CharacterBase
{
public:
	Enemy(DirectX11& dx, const ECharacterId& CharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

	virtual void TurnElapsed() override {};

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
};