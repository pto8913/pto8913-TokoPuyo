#pragma once

#include "Object/Object.h"

#include "Algorithm/Array.h"

#include "Engine/Timer.h"
#include "Math/Vector.h"

#include "Framework/Level/ObjectManager.h"

class DirectX11;
class World;

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
class Level : public Object, public std::enable_shared_from_this<Level>
{
public:
	Level(DirectX11& dx);
	virtual ~Level();

protected:
	virtual void SetObjectManager();

public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	void SetWorld(std::shared_ptr<World> in);
	virtual std::shared_ptr<World> GetWorld() override;

	ObjectManager* GetObjectManager();

public:
	// ---------------------------
	// Main : Util
	// ---------------------------
	FVector GetStartPosition();
	void SetStartPosition(FVector in);

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Level" + "#"s + std::to_string(mID);
	}
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	DirectX11* pDX = nullptr;

	std::shared_ptr<World> pOwningWorld = nullptr;

	std::shared_ptr<ObjectManager> pObjectManager = nullptr;

	FVector mStartPosition;
};
