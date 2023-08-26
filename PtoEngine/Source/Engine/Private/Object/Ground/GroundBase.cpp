
#include "Object/Ground/GroundBase.h"

#include <format>

struct FGroundSettings : public FActor2DSettings
{
	FGroundSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize)
	{
	}

	FGroundSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag)
	{
	}
};

const std::map<EGroundId, FGroundSettings> GroundSettings =
{
	{EGroundId::Room_Plain, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Room_Plain")},
	{EGroundId::Room_Forest, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Room_Forest")},
	{EGroundId::Room_Cave, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Room_Cave")},
	{EGroundId::Room_Water, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Room_Water")},

	{EGroundId::Wall_Plain, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Wall_Plain")},
	{EGroundId::Wall_Forest, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Wall_Forest")},
	{EGroundId::Wall_Cave, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Wall_Cave")},
	{EGroundId::Wall_Water, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Wall_Water")},

	{EGroundId::Path_Plain, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Path_Plain")},
	{EGroundId::Path_Forest, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Path_Forest")},
	{EGroundId::Path_Cave, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Path_Cave")},
	{EGroundId::Path_Water, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Path_Water")},

	{EGroundId::None, FGroundSettings(L"Content/Textures/T_Cave_Wall_0.png", L"None")},
};

GroundBase::GroundBase(DirectX11& dx, const EGroundId& inGroundType)
	: Actor2D(
		dx, 
		GroundSettings.at(inGroundType),
		1.f
	),
	GroundType(inGroundType)
{
	mSortOrder = Layer::Ground;
	mLayer = EActor2DLayer::Background;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GroundBase::SetGroundType(const EGroundId& inGroundType)
{
	UpdateTexture(GroundSettings.at(inGroundType).fileName);
	GroundType = inGroundType;
}
const EGroundId& GroundBase::GetGroundType() const noexcept
{
	return GroundType;
}