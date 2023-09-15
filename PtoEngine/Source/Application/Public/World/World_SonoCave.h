#pragma once

#include "World/World_Dungeon.h"

class World_SonoCave : public World_Dungeon
{
public:
	World_SonoCave();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(World_SonoCave).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
	virtual void SetLevel(DirectX11& dx) override;
	virtual void SetHUD(DirectX11& dx) override;
};