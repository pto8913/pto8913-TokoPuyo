#pragma once

#include "Object/Object.h"

class Actor;

class ComponentBase abstract : public Object
{
public:
	ComponentBase(Actor* inOwner);
	virtual ~ComponentBase();

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