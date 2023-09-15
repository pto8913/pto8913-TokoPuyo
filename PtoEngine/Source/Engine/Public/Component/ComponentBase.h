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
	virtual World* GetWorld() override final;

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(ComponentBase).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	Actor* pOwner = nullptr;
};