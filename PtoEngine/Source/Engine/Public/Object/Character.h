#pragma once

#include "Actor/Actor2D.h"
#include "Actor/Character/CharacterTypes.h"

#include "Engine/Delegate.h"

struct FCharacterSettings;

class MovementComponent;
class BoxCollision2D;

class Character : public Actor2D, public std::enable_shared_from_this<Character>
{
public:
	Character(DirectX11& dx, const ECharacterId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

	void SetCharacterType(const ECharacterId& inCharacterType);
	const ECharacterId& GetCharacterType() const noexcept;

	std::shared_ptr<MovementComponent> GetMovementComp();

	virtual void TurnElapsed() {};

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	ECharacterId characterType;

	std::shared_ptr<MovementComponent> pMovementComponent = nullptr;
	std::shared_ptr<BoxCollision2D> pBoxCollision2D = nullptr;
};
