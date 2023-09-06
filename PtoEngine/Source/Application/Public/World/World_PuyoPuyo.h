#pragma once

#include "Framework/World.h"

class World_PuyoPuyo : public World
{
public:
	World_PuyoPuyo();

	virtual void SetLevel(DirectX11& dx) override;
	virtual void SetHUD(DirectX11& dx) override;
};