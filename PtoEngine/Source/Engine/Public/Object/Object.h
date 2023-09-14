#pragma once

#include <memory>
#include <Windows.h>
#include "Engine/Delegate.h"
#include "Framework/Level/Layer.h"
#include <unordered_set>

class DirectX11;

class World;

class UserWidget;

class Object
{
	friend class World;
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
private:
	/* This will be call from World->SpawnActor() only once */
	virtual void SetID(int inID);
public:
	int GetID() const;

	void AddTag(const std::wstring& inTag);
	void RemoveTag(const std::wstring& inTag);
	bool HasTag(const std::wstring& inTag) const;

	template<class TClass, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<UserWidget, TClass>>>
	TClass* CreateWidget(Object* inOwner, Args&& ...args)
	{
		auto out = std::make_shared<TClass>(inOwner, std::forward<Args>(args)...);
		GetWorld()->AddToObjectCollection(out);
		return std::move(out.get());
	};

	virtual std::string GetName() const
	{
		using namespace std::string_literals;
		return typeid(Object).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	bool bEnableTick = true;
	bool bIsPendingKill = false;
protected:
	Layer::EActorLayer mLayer = Layer::EActorLayer::Default;
	int mID = -1;
	std::unordered_set<std::wstring> mTags;
};

bool IsValid(std::shared_ptr<Object> in);
bool IsValid(Object* in);
