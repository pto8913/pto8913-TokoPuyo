#pragma once

#include "Graphics/Object/Sprite.h"
#include "Component/ActorComponent.h"

class SpriteComponent : public ActorComponent, public Sprite
{
public:
	SpriteComponent(Actor* inOwner, DirectX11& dx, const std::wstring& inFileName, std::wstring inTag, DirectX::XMFLOAT2 inSize);
	virtual ~SpriteComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "SpriteComponent" + "#"s + std::to_string(mID);
	}
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual FVector GetActorLocation() override;
	virtual void SetActorLocation(const FVector& in) override;
	virtual void AddActorLocation(const FVector& in) override;
	virtual FRotator GetActorRotation() override;
	virtual void SetActorRotation(const FRotator& in) override;
	virtual FVector GetActorScale() override;
	virtual void SetActorScale(const FVector& in) override;

};
