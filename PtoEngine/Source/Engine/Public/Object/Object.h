#pragma once

#include <memory>
#include <Windows.h>
#include "Engine/Delegate.h"
#include "Framework/Level/Layer.h"
#include <unordered_set>

class DirectX11;

class World;

class Object
{
	friend class WidgetManager;
	friend class World;
public:
	Object();
	virtual ~Object();

	/* Will be called when the between Constructor to BeginPlay */
	virtual void NativeOnInitialized(DirectX11& dx) {};

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) {};
	virtual void EndPlay() {};

	// called per frame.
	virtual void Tick(DirectX11& dx, float deltaTime) {};

	virtual void SetTickEnabled(bool inState) noexcept;
	virtual bool GetTickEnabled() const noexcept;

	virtual World* GetWorld() { return nullptr; }

	bool IsPendingKill() const noexcept;
	void MarkPendingKill();

	const Layer::EActorLayer& GetLayer() const;
	void SetLayer(const Layer::EActorLayer& in);
protected:
	/* This will be call from World->SpawnActor() only once */
	virtual void SetID(int inID);
public:
	int GetID() const;

	void AddTag(const std::wstring& inTag);
	void RemoveTag(const std::wstring& inTag);
	bool HasTag(const std::wstring& inTag) const;

	virtual std::string GetName() const
	{
		using namespace std::string_literals;
		return "Object" + "#"s + std::to_string(mID);
	}
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	bool bEnableTick = true;
	bool bIsPendingKill = false;
protected:
	Layer::EActorLayer mLayer = Layer::EActorLayer::Default;
	int mID = -1;
	std::unordered_set<std::wstring> mTags;
	std::string mName;
};

bool IsValid(std::shared_ptr<Object> in);
bool IsValid(Object* in);
