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

	GameStateBase(const GameStateBase&) = delete;
	GameStateBase& operator=(const GameStateBase&) = delete;
};