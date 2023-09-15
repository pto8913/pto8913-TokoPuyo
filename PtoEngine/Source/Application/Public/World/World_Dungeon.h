#pragma once

#include "Framework/World.h"

class World_Dungeon : public World
{
public:
	World_Dungeon();
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(World_Dungeon).name() + std::string("_") + std::to_string(mID);
	}
	virtual void SetGameMode(DirectX11& dx) override;
	virtual void SetGameState(DirectX11& dx) override;
	virtual void SetPlayerController(DirectX11& dx) override;
	virtual void SetPlayer(DirectX11& dx) override;
	virtual void SetHUD(DirectX11& dx) override;
protected:
	std::wstring mDungeonName = L"";
};
