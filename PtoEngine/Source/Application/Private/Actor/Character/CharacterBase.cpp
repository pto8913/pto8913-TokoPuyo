
#include "Actor/Character/CharacterBase.h"

#include "Component/MovementComponent.h"
#include "Component/BoxCollision2D.h"
#include "Component/SpriteComponent.h"

class CharacterManager
{
public:
	CharacterManager() = default;

	CharacterManager(CharacterManager& other) = delete;
	CharacterManager& operator=(CharacterManager& other) = delete;

	static CharacterManager& Get()
	{
		static CharacterManager instance;
		return instance;
	}

private:
	UINT8 maxSpawnEnemy = 6;
};

struct FCharacterSettings : public FActor2DSettings
{
public:
	FCharacterSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize, Layer::Character)
	{
	}

	FCharacterSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag, Layer::Character)
	{
	}
};

const std::map<ECharacterId, FCharacterSettings> CharacterList =
{
	{ECharacterId::Player, FCharacterSettings(L"Content/Textures/T_Puyo_Red.png", L"Player")},
};

CharacterBase::CharacterBase(DirectX11& dx, const ECharacterId& inCharacterType)
	: Actor2D(
		dx,
		CharacterList.at(inCharacterType),
		1.f
	),
	characterType(inCharacterType)
{
	pMovementComponent = AddComponent<MovementComponent>("Movement", this);
	pBoxCollision2D = AddComponent<BoxCollision2D>("Collision2D", this);

	mLayer = Layer::EActorLayer::Entities;
	mSortOrder = Layer::Character;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void CharacterBase::SetCharacterType(const ECharacterId& inCharacterType)
{
	GetSpriteComp()->UpdateTexture(CharacterList.at(inCharacterType).fileName);
	characterType = inCharacterType;
}
const ECharacterId& CharacterBase::GetCharacterType() const noexcept
{
	return characterType;
}

std::shared_ptr<MovementComponent> CharacterBase::GetMovementComp()
{
	return pMovementComponent;
}

CharacterManager& CharacterBase::GetManager()
{
	return CharacterManager::Get();
}
