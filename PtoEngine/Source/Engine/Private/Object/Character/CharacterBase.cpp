
#include "Object/Character/CharacterBase.h"

#include "GameSettings.h"

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

struct FCharacterSettings : public FLayerObject2DSettings
{
public:
	FCharacterSettings(const std::wstring& inFileName, const std::wstring& inTag, const DirectX::XMFLOAT2& inSize)
		: FLayerObject2DSettings(inFileName, inTag, inSize)
	{
	}

	FCharacterSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FLayerObject2DSettings(inFileName, inTag)
	{
	}
};

const std::map<ECharacterId, FCharacterSettings> CharacterList =
{
	{ECharacterId::Player, FCharacterSettings(L"Content/Textures/T_Puyo_Red.png", L"Player")},
};

CharacterBase::CharacterBase(DirectX11& dx, const ECharacterId& inCharacterType)
	: LayerObject2DBase(
		dx,
		CharacterList.at(inCharacterType),
		4, 
		1.f
	),
	characterType(inCharacterType)
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void CharacterBase::SetCharacterType(const ECharacterId& inCharacterType)
{
	UpdateTexture(CharacterList.at(inCharacterType).fileName);
	characterType = inCharacterType;
}
const ECharacterId& CharacterBase::GetCharacterType() const noexcept
{
	return characterType;
}

CharacterManager& CharacterBase::GetManager()
{
	return CharacterManager::Get();
}