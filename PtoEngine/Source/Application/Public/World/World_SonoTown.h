#pragma once

#include "World/World_Town.h"

class World_SonoTown : public World_Town
{
public:
	World_SonoTown();

	virtual std::string GetName() const override
	{
		return "World_SonoTown_" + std::to_string(mID);
	}
	virtual void SetLevel(DirectX11& dx) override;
};