#pragma once

#include "Actor/Actor2D.h"
#include "BuildingTypes.h"

#include "Engine/Delegate.h"

class BuildingBase : public Actor2D
{
public:
	BuildingBase(DirectX11& dx, const EBuildingId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetBuildingType(const EBuildingId& inBuildingType);
	const EBuildingId& GetBuildingType() const noexcept;
		
	virtual std::string GetName() const override
	{
		return "BuildingBase_" + std::to_string(mID);
	}
protected:
	EBuildingId mBuildingType;
};