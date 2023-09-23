#pragma once

#include "Level2D.h"

class DirectX11;
class GameState_Play;

class Level_TokoPuyo : public Level2D
{
public:
	Level_TokoPuyo(DirectX11& dx);
	virtual ~Level_TokoPuyo();

	// ------------------------------------------------------------
	// Main
	// ------------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Level_TokoPuyo" + "#"s + std::to_string(mID);
	}
protected:
	virtual void GenerateGroundLayer() override;
public:

	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	GameState_Play* pGameState = nullptr;
};