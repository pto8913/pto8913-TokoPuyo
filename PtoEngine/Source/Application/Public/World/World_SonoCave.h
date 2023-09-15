#pragma once

#include "World/World_Dungeon.h"

class World_SonoCave : public World_Dungeon
{
public:
	World_SonoCave();

	virtual std::string GetName() const override
	{
		return "World_SonoCave_" + std::to_string(mID);
	}
	virtual void SetLevel(DirectX11& dx) override;
	virtual void SetHUD(DirectX11& dx) override;
};