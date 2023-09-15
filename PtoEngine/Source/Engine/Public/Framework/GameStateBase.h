#pragma once

#include "Object/Actor.h"


class DirectX11;

class GameStateBase : public Actor
{
public:
	GameStateBase();
	virtual ~GameStateBase();

	GameStateBase(const GameStateBase&) = delete;
	GameStateBase& operator=(const GameStateBase&) = delete;

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(GameStateBase).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
};