#pragma once

#include "Object/Object.h"

#include "Algorithm/Array.h"

#include "Engine/Timer.h"
#include "Engine/Vector.h"

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

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetWorld(std::shared_ptr<World> in);
	virtual std::shared_ptr<World> GetWorld() override;

	// ------------------------------------------------------
	// Main : Util
	// ------------------------------------------------------
	FVector GetStartPosition();
	void SetStartPosition(FVector in);

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	DirectX11* pDX = nullptr;

	std::shared_ptr<World> pOwningWorld = nullptr;

	FVector mStartPosition;
};

