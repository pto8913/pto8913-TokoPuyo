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
	/* Change Texture and GroundType. */
	void SetGroundType(const EGroundId& inGroundType);
	const EGroundId& GetGroundType() const noexcept;

	void SetNormal(const FVector2D& in);
	FVector2D GetNormal() const;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	EGroundId mGroundType;
	FVector2D mNormal;

	std::shared_ptr<BoxCollision2D> pBoxCollision = nullptr;
};