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
	/* Change Texture and GroundType. */
	void SetGroundType(const EGroundId& inGroundType);
	const EGroundId& GetGroundType() const noexcept;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	EGroundId mGroundType;
};