#pragma once

#include "Object/LayerObjectBase.h"
#include "CharacterTypes.h"

#include "Engine/Delegate.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChracterMoved, int /* Id */, DirectX::XMFLOAT2 /* coords */)

class CharacterManager;
struct FCharacterSettings;

struct FCharacterStatus
{
public:
	float health;
	float stamina;
	float mana;

	float regist_all_abnormal;

};

class CharacterBase : public LayerObject2DBase
{
public:
	CharacterBase(DirectX11& dx, const ECharacterId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetCharacterType(const ECharacterId& inCharacterType);
	const ECharacterId& GetCharacterType() const noexcept;

	virtual void TurnElapsed() {};
protected:
	CharacterManager& GetManager();
public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	ECharacterId characterType;
	
	FOnChracterMoved OnChracterMoved;
};
