#pragma once

#include "Object/Object.h"

class Actor;

class ActorComponent : public Object
{
public:
	ActorComponent(std::shared_ptr<Actor> inOwner);
	virtual ~ActorComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	std::shared_ptr<Actor> GetOwner();

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::shared_ptr<Actor> pOwner = nullptr;
};