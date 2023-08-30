#pragma once

#include "Object/Object.h"

class Actor;

class ActorComponent : public Object
{
public:
	ActorComponent(Actor* inOwner);
	virtual ~ActorComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	Actor* GetOwner();
	virtual std::shared_ptr<World> GetWorld() override final;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	Actor* pOwner = nullptr;
};