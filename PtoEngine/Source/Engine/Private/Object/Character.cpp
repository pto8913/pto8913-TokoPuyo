
#include "Object/Character.h"

#include "Component/MovementComponent.h"
#include "Component/BoxCollision2D.h"

struct FCharacterSettings : public FActor2DSettings
{
public:
	FCharacterSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize)
	{
	}

	FCharacterSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag)
	{
	}
};

const std::map<ECharacterId, FCharacterSettings> CharacterList =
{
	{ECharacterId::Player, FCharacterSettings(L"Content/Textures/T_Puyo_Red.png", L"Player")},
};

Character::Character(DirectX11& dx, const ECharacterId& inCharacterType)
	: Actor2D(
		dx,
		CharacterList.at(inCharacterType),
		1.f
	),
	characterType(inCharacterType)
{
	pMovementComponent = AddComponent<MovementComponent>("Movement", this);
	pBoxCollision2D = AddComponent<BoxCollision2D>("Collision2D", this);

	mSortOrder = Layer::Character;
	mLayer = Layer::EActorLayer::Entities;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Character::SetCharacterType(const ECharacterId& inCharacterType)
{
	UpdateTexture(CharacterList.at(inCharacterType).fileName);
	characterType = inCharacterType;
}
const ECharacterId& Character::GetCharacterType() const noexcept
{
	return characterType;
}

std::shared_ptr<MovementComponent> Character::GetMovementComp()
{
	return pMovementComponent;
}