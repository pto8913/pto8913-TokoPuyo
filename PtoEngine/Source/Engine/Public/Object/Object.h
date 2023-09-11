#pragma once

#include <memory>
#include <Windows.h>
#include "Engine/Delegate.h"
#include "Framework/Level/Layer.h"

class DirectX11;

class World;

class UserWidget;

class Object
{
public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) {};
	virtual void EndPlay() {};

	// called per frame.
	virtual void Tick(DirectX11& dx, float deltaTime) {};

	void SetTickEnabled(bool inState) noexcept;
	bool GetTickEnabled() const noexcept;

	virtual World* GetWorld() { return nullptr; }

	bool IsPendingKill() const noexcept;
	void MarkPendingKill();

	const Layer::EActorLayer& GetLayer() const;
	void SetLayer(const Layer::EActorLayer& in);

	template<class TClass, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<UserWidget, TClass>>>
	std::shared_ptr<TClass>&& CreateWidget(Object* inOwner, Args&& ...args)
	{
		auto out = std::make_shared<TClass>(inOwner, std::forward<Args>(args)...);
		GetWorld()->AddToObjectCollection(out);
		return std::move(out);
	};
private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	bool bEnableTick = true;
	bool bIsPendingKill = false;
protected:
	Layer::EActorLayer mLayer;

};

bool IsValid(std::shared_ptr<Object> in);
bool IsValid(Object* in);
