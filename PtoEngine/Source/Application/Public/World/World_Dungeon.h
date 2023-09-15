#pragma once

#include "Framework/World.h"

class World_Dungeon : public World
{
public:
	World_Dungeon();
	virtual std::string GetName() const override
	{
		return "World_Dungeon_" + std::to_string(mID);
	}
	virtual void SetGameMode(DirectX11& dx) override;
	virtual void SetGameState(DirectX11& dx) override;
	virtual void SetPlayerController(DirectX11& dx) override;
	virtual void SetPlayer(DirectX11& dx) override;
	virtual void SetHUD(DirectX11& dx) override;
protected:
	std::wstring mDungeonName = L"";
};
