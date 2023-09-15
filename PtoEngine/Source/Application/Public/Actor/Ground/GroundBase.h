#pragma once

#include "Actor/Actor2D.h"
#include "GroundTypes.h"

class DirectX11;

class GroundBase : public Actor2D
{
public:
	GroundBase(DirectX11& dx, const EGroundId& inGroundType);
	virtual ~GroundBase();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(GroundBase).name() + std::string("_") + std::to_string(mID);
	}

	/* Change Texture and GroundType. */
	void SetGroundType(const EGroundId& inGroundType);
	const EGroundId& GetGroundType() const noexcept;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	EGroundId mGroundType;
};