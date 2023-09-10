#pragma once

#include "Object/Object.h"

class Actor;

class ComponentBase : public Object
{
public:
	ComponentBase(Actor* inOwner);
	virtual ~ComponentBase();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	Actor* GetOwner();
	virtual World* GetWorld() override final;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	Actor* pOwner = nullptr;
};