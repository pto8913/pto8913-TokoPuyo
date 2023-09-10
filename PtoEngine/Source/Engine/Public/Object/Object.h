#pragma once

#include <memory>
#include <Windows.h>
#include "Engine/Delegate.h"

class DirectX11;

class World;

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

private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	bool bEnableTick = true;
};