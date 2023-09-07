
#include "Actor/Character/Puyo.h"
#include "Component/SpriteComponent.h"

struct FPuyoSettings : public FActor2DSettings
{
public:
	FPuyoSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize, Layer::Character)
	{
	}

	FPuyoSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag, Layer::Character)
	{
	}
};

const std::map<uint8_t, FPuyoSettings> PuyoList =
{
	{0, FPuyoSettings(L"Content/Textures/T_Puyo_Red.png", L"Red")},
	{1, FPuyoSettings(L"Content/Textures/T_Puyo_Green.png", L"Green")},
	{2, FPuyoSettings(L"Content/Textures/T_Puyo_Yellow.png", L"Yellow")},
	{3, FPuyoSettings(L"Content/Textures/T_Puyo_Purple.png", L"Purple")},
	{4, FPuyoSettings(L"", L"Invalid")},
	{5, FPuyoSettings(L"", L"Invalid2")},
};

Puyo::Puyo(DirectX11& dx, const uint8_t& id)
	: Actor2D(dx, PuyoList.at(id))
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Puyo::SetType(const uint8_t& type)
{
	mType = type;
	GetSpriteComp()->UpdateTexture(PuyoList.at(mType).fileName);
}
uint8_t Puyo::GetType() const
{
	return mType;
}

Puyo::Rotation Puyo::GetRotation()
{
	return mRotation;
}
void Puyo::SetRotation(const Puyo::Rotation& in)
{
	mRotation = in;
}

bool Puyo::GetIsActive()
{
	return bIsActive;
}
void Puyo::SetIsActive(const bool& in)
{
	bIsActive = in;
}

bool Puyo::IsSame(const std::shared_ptr<Puyo>& in)
{
	return GetType() == in->GetType() && Get2DIdx() == in->Get2DIdx();
}
bool Puyo::IsSameType(const std::shared_ptr<Puyo>& in)
{
	return GetType() == in->GetType();
}