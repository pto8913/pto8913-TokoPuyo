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

	void SetID(int inID);
	int GetID() const;

	template<typename TClass, typename ...Args>
	std::shared_ptr<TClass> AddComponent(const std::string& tag, Args&& ...args)
	{
		auto ptr = std::make_shared<TClass>(std::forward<Args>(args)...);
		pComponents.emplace(tag, ptr);
		return ptr;
	}
	void RemoveComponent(const std::string& tag);

	template<typename TClass>
	std::shared_ptr<TClass> GetComponent()
	{
		for (auto&& comp : pComponents)
		{
			if (comp.second != nullptr)
			{
				if (auto ptr = static_pointer_cast<TClass>(comp.second))
				{
					return ptr;
				}
			}
		}
		return nullptr;
	}

	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual FVector GetActorLocation();
	virtual void SetActorLocation(const FVector& in);
	virtual void AddActorLocation(const FVector& in);
	virtual FRotator GetActorRotation();
	virtual void SetActorRotation(const FRotator& in);
	virtual FVector GetActorScale();
	virtual void SetActorScale(const FVector& in);

	FVector GetActorForwardVector();
	FVector GetActorUpVector();
	FVector GetActorRightVector();

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
	int mID = -1;

	FVector mLocation;
	FRotator mRotation;
	FVector mScale;

	std::map<std::string, std::shared_ptr<ActorComponent>> pComponents;
};