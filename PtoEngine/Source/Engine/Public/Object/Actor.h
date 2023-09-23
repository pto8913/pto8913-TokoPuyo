#pragma once

#include "Object.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"

#include <map>

class DirectX11;

class Level;
class World;
class ComponentBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestroyed, std::shared_ptr<class Actor>);

class Actor : public Object, public std::enable_shared_from_this<Actor>
{
	friend class World;
public:
	Actor();
	virtual ~Actor();

	/* Will be called when the between Constructor to BeginPlay */
	virtual void NativeOnInitialized() = 0;

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void EndPlay() override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;
	/* 
	* Call remove event to world.
	*/
	void DestroyActor();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Actor" + "#"s + std::to_string(mID);
	}

	// -----------------------------------
	// Main : Component
	// -----------------------------------
	template<typename TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<ComponentBase, TClass>>>
	TClass* AddComponent(const std::string& tag, Args&& ...args)
	{
		auto ptr = std::make_shared<TClass>(std::forward<Args>(args)...);
		pComponents.emplace(tag, ptr);
		return std::move(ptr.get());
	}
	void RemoveComponent(const std::string& tag);

	template<typename TClass>
	TClass* GetComponent()
	{
		for (auto&& comp : pComponents)
		{
			if (comp.second != nullptr)
			{
				if (auto ptr = static_pointer_cast<TClass>(comp.second))
				{
					return ptr.get();
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
private:
	/* This will be call from World->SpawnActor() only once */
	void SetOuter(Object* inOuter);
public:
	Object* GetOuter();
	
	Level* GetLevel();
	virtual World* GetWorld() override final;

	template<typename T>
	T* GetTypedOuter()
	{
		return static_cast<T*>(pOuter);
	}

	// -----------------------------------
	// Main : Delegates
	// -----------------------------------
	FOnDestroyed OnDestroyed;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	Object* pOuter = nullptr;
	/* NOTE :
		Is not same Actor::mLocation to ObjectBase::location.
		Actor::mLocation is location in screen.
		ObjectBase::location is location in window. this is include menubar, window title...etc.
	*/
	FVector mLocation;
	FRotator mRotation;
	FVector mScale;

	std::map<std::string, std::shared_ptr<ComponentBase>> pComponents;
};