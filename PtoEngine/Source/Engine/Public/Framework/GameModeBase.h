#pragma once

#include "Object/Actor.h"

class GameModeBase : public Actor
{
public:
	GameModeBase();
	virtual ~GameModeBase();

	GameModeBase(const GameModeBase&) = delete;
	GameModeBase& operator=(const GameModeBase&) = delete;


};