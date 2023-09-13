
#include "Actor/Ground/GroundBase.h"

#include "Component/SpriteComponent.h"

struct FGroundSettings : public FActor2DSettings
{
	FGroundSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize, Layer::Ground)
	{
	}

	FGroundSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag, Layer::Ground)
	{
	}
};

const std::map<EGroundId, FGroundSettings> GroundSettings =
{
	{EGroundId::None, FGroundSettings(L"Content/Textures/T_Cave_Wall_0.png", L"None")},
	{EGroundId::Background, FGroundSettings(L"Content/Textures/T_Grid_Cell_30x30.png", L"Background")},
	
};

GroundBase::GroundBase(DirectX11& dx, const EGroundId& inGroundType)
	: Actor2D(
		dx, 
		GroundSettings.at(inGroundType),
		1.f
	)
{
	mLayer = Layer::EActorLayer::Background;
	mSortOrder = Layer::Ground;

	SetGroundType(inGroundType);
}
GroundBase::~GroundBase()
{

}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GroundBase::SetGroundType(const EGroundId& inGroundType)
{
	mGroundType = inGroundType;
	GetSpriteComp()->UpdateTexture(GroundSettings.at(mGroundType).fileName);
}
const EGroundId& GroundBase::GetGroundType() const noexcept
{
	return mGroundType;
}