#pragma once

#include "Actor/Actor2D.h"
#include "GroundTypes.h"

class DirectX11;

class BoxCollision2D;

class GroundBase : public Actor2D
{
public:
	GroundBase(DirectX11& dx, const EGroundId& inGroundType);

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

	void SetNormal(const EDirection& in);
	EDirection GetNormal() const;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	EGroundId mGroundType;
	EDirection mNormal = EDirection::InValid;

	std::shared_ptr<BoxCollision2D> pBoxCollision = nullptr;
};