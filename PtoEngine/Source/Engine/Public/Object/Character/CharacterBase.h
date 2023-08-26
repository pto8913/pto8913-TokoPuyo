#pragma once

#include "Object/Actor2D.h"
#include "Object/Character/CharacterTypes.h"

#include "Engine/Delegate.h"

class CharacterManager;
struct FCharacterSettings;

class MovementComponent;
class BoxCollision;

struct FCharacterStatus
{
public:
	float health;
	float stamina;
	float mana;

	float regist_all_abnormal;

};

class CharacterBase : public Actor2D, public std::enable_shared_from_this<CharacterBase>
{
public:
	CharacterBase(DirectX11& dx, const ECharacterId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

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
	std::shared_ptr<BoxCollision> pBoxCollision = nullptr;
};
