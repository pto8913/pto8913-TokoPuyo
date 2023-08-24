#pragma once

#include "Object/LayerObjectBase.h"
#include "Level/Layer/CharacterTypes.h"

#include "Engine/Delegate.h"

class CharacterManager;
struct FCharacterSettings;

class MovementComponent;

struct FCharacterStatus
{
public:
	float health;
	float stamina;
	float mana;

	float regist_all_abnormal;

};

class CharacterBase : public LayerObject2DBase, public std::enable_shared_from_this<CharacterBase>
{
public:
	CharacterBase(DirectX11& dx, const ECharacterId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;

	void SetCharacterType(const ECharacterId& inCharacterType);
	const ECharacterId& GetCharacterType() const noexcept;

	std::shared_ptr<MovementComponent> GetMovementComp();

	virtual void TurnElapsed() {};

protected:
	CharacterManager& GetManager();
public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	ECharacterId characterType;

	std::shared_ptr<MovementComponent> pMovementComponent = nullptr;
};
