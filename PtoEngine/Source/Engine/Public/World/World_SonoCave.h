#pragma once

#include "Engine/World.h"

class World_SonoCave : public World
{
public:
	World_SonoCave();

	virtual void SetLevel(DirectX11& dx) override;
	virtual void SetGameMode(DirectX11& dx) override;
	virtual void SetGameState(DirectX11& dx) override;
	virtual void SetPlayerController(DirectX11& dx) override;

private:
	std::wstring mDungeonName = L"";
};