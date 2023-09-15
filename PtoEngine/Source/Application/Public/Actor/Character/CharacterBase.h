#pragma once

#include "Actor/Actor2D.h"
#include "Actor/Character/CharacterTypes.h"

#include "Engine/Delegate.h"

class CharacterManager;
struct FCharacterSettings;

class MovementComponent;
class BoxCollision2D;

class CharacterBase : public Actor2D, public std::enable_shared_from_this<CharacterBase>
{
public:
	CharacterBase(DirectX11& dx, const ECharacterId& inCharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(CharacterBase).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}

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
	std::shared_ptr<BoxCollision2D> pBoxCollision2D = nullptr;
};
