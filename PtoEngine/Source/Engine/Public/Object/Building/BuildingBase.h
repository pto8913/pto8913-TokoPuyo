#pragma once

#include "Object/LayerObjectBase.h"
#include "Level/Layer/BuildingTypes.h"

#include "Engine/Delegate.h"

class BuildingBase : public LayerObject2DBase
{
public:
	BuildingBase(DirectX11& dx, const EBuildingId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetBuildingType(const EBuildingId& inBuildingType);
	const EBuildingId& GetBuildingType() const noexcept;
		
protected:
	EBuildingId mBuildingType;
};