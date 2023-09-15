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
		return "GroundBase_" + std::to_string(mID);
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