#pragma once

#include "World/World_Dungeon.h"

class World_SonoCave : public World_Dungeon
{
public:
	World_SonoCave();

	virtual void SetLevel(DirectX11& dx) override;
	virtual void SetHUD(DirectX11& dx) override;
};