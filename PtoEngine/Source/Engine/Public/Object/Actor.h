#pragma once

#include "Object.h"
#include "Engine/Vector.h"
#include "Engine/Rotator.h"

#include <map>

class DirectX11;

class Level;
class World;
class ActorComponent;

class Actor : public Object
{
public:
	Actor();
	virtual ~Actor();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	template<typename TClass, typename ...Args>
	std::shared_ptr<TClass> AddComponent(const std::string& tag, Args&& ...args)
	{
		auto ptr = std::make_shared<TClass>(std::forward<Args>(args)...);
		pComponents.emplace(tag, ptr);
		return ptr;
	}
	void RemoveComponent(const std::string& tag);

	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual FVector GetActorLocation();
	virtual void SetActorLocation(const FVector& in);
	virtual FRotator GetActorRotation();
	virtual void SetActorRotation(const FRotator& in);
	virtual FVector GetActorScale();
	virtual void SetActorScale(const FVector& in);

	// -----------------------------------
	// Main : Util
	// -----------------------------------
	virtual void SetOuter(std::shared_ptr<Object> inOuter);
	std::shared_ptr<Object> GetOuter();
	
	std::shared_ptr<Level> GetLevel();
	virtual std::shared_ptr<World> GetWorld() override final;

	template<typename T>
	std::shared_ptr<T> GetTypedOuter()
	{
		return std::static_pointer_cast<T>(pOuter);
	}
protected:
	std::shared_ptr<Object> pOuter = nullptr;

	FVector mLocation;
	FRotator mRotation;
	FVector mScale;

	std::map<std::string, std::shared_ptr<ActorComponent>> pComponents;
};