
#include "Framework/GameInstance.h"
#include "Framework/World.h"

GameInstance::~GameInstance()
{
	pWorld.reset();
	pWorld = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void GameInstance::Initialize(DirectX11& dx)
{
}
void GameInstance::Tick(DirectX11& dx, float deltaSec)
{
	if (mTimerManager)
	{
		mTimerManager.Tick();
	}

	if (pWorld != nullptr)
	{
		if (!pWorld->IsPendingKill())
		{
			pWorld->Tick(dx, deltaSec);
		}
	}
}

// -----------------------------------
// Main : Utils
// -----------------------------------
GameInstance& GameInstance::Get()
{
	static GameInstance instance;
	return instance;
}
TimerManager& GameInstance::GetTimerManager()
{
	return mTimerManager;
}

void GameInstance::OpenWorld(int id)
{
	mQueuedOpenWorldID = id;

	if (pWorld != nullptr)
	{
		pWorld->MarkPendingKill();

		FTimerDelegate openWorldDelay;
		openWorldDelay.SetTimer<&GameInstance::OpenWorldDelay>(*this);
		GetTimerManager().SetNextTickTimer(openWorldDelay);
	}
	else
	{
		OpenWorldDelay();
	}
}

void GameInstance::OpenWorldDelay()
{
	// auto openWorldID = static_cast<EWorldID>(mQueuedOpenWorldID);
	// switch (openWorldID)
	// {
	// case EWorldID::Title:
	//     pWorld = std::make_shared<World>();
	//     break;
	// default:
	//     break;
	// };
	// OnOpenWorld.Broadcast(pWorld);
}

