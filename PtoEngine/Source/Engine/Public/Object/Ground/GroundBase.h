#pragma once

#include "Object/LayerObjectBase.h"
#include "Level/Layer/GroundTypes.h"

class DirectX11;

class GroundBase : public LayerObject2DBase
{
public:
	GroundBase(DirectX11& dx, const EGroundId& inGroundType);

	/* Change Texture and GroundType. */
	void SetGroundType(const EGroundId& inGroundType);
	const EGroundId& GetGroundType() const noexcept;

private:
	EGroundId GroundType;
};