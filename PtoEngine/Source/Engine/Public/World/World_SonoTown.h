#pragma once

#include "World/World_Town.h"

class World_SonoTown : public World_Town
{
public:
	World_SonoTown();

	virtual void SetLevel(DirectX11& dx) override;
};