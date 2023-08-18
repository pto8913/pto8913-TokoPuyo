#pragma once

#include "Engine/World.h"

class World_SonoCave : public World
{
public:
	World_SonoCave(DirectX11& dx);
	virtual ~World_SonoCave();

	virtual void SetGameState(DirectX11& dx) override;
	virtual void SetPlayerController(DirectX11& dx) override;
	virtual void SetLevel(DirectX11& dx) override;
};