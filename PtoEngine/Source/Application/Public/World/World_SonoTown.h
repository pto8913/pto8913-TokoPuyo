#pragma once

#include "World/World_Town.h"

class World_SonoTown : public World_Town
{
public:
	World_SonoTown();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(World_SonoTown).name() + std::string("_") + std::to_string(mID);
	}
	virtual void SetLevel(DirectX11& dx) override;
};