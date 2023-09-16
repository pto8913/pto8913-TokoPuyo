#pragma once

#include "Framework/World.h"

class World_TokoPuyo : public World
{
public:
	World_TokoPuyo();
	virtual ~World_TokoPuyo();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "World_TokoPuyo" + "#"s + std::to_string(mID);
	}
	virtual void SetLevel(DirectX11& dx) override;
	virtual void SetGameMode(DirectX11& dx) override;
	virtual void SetGameState(DirectX11& dx) override;
	virtual void SetPlayerController(DirectX11& dx) override;
};