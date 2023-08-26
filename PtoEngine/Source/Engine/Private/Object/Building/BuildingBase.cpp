
#include "Object/Building/BuildingBase.h"

struct FBuildingSettings : public FActor2DSettings
{
public:
	FBuildingSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize)
	{
	}

	FBuildingSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag)
	{
	}
};

const std::map<EBuildingId, FBuildingSettings> BuildingList =
{
	{EBuildingId::SonoTown_0, FBuildingSettings(L"Content/Textures/T_House_SonoTown_0.png", L"House_SonoTown_0")},
};

BuildingBase::BuildingBase(DirectX11& dx, const EBuildingId& inBuildingType)
	: Actor2D(
		dx,
		BuildingList.at(inBuildingType),
		1.f
	),
	mBuildingType(inBuildingType)
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void BuildingBase::SetBuildingType(const EBuildingId& inBuildingType)
{
	UpdateTexture(BuildingList.at(inBuildingType).fileName);
	mBuildingType = inBuildingType;
}
const EBuildingId& BuildingBase::GetBuildingType() const noexcept
{
	return mBuildingType;
}
