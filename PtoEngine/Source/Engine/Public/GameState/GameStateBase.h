#pragma once

#include "Object/Actor.h"

class DirectX11;

/*
* 
* 
* 
* 
*/

class GameStateBase : public Actor
{
public:
	GameStateBase();
	virtual ~GameStateBase();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void EndPlay() override;

	virtual void Tick(DirectX11& dx, float deltaTime) override;

	virtual void RestartGame(DirectX11& dx);
	virtual void GenerateWorld(DirectX11& dx) = 0;
};