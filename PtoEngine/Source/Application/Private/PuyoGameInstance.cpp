
#include "PuyoGameInstance.h"

#include "World/World_Title.h"
#include "World/World_TokoPuyo.h"

PuyoGameInstance::~PuyoGameInstance()
{
}

PuyoGameInstance& PuyoGameInstance::Get()
{
	static PuyoGameInstance instance;
	return instance;
}

void PuyoGameInstance::OpenWorld(World* currentWorld, const EWorldId& id)
{
	OpenWorldID = id;
	if (currentWorld != nullptr)
	{
		currentWorld->MarkPendingKill();
		currentWorld->GetTimerManager().SetTimer<&PuyoGameInstance::OpenWorldDelay>(
			*this,
			0, 0, 0.01f
		);
	}
	else
	{
		OpenWorldDelay();
	}
}

void PuyoGameInstance::OpenWorldDelay()
{
	switch (OpenWorldID)
	{
	case EWorldId::Title:
		OnOpenWorld.Broadcast(std::make_shared<World_Title>());
		break;
	case EWorldId::TokoPuyo:
		OnOpenWorld.Broadcast(std::make_shared<World_TokoPuyo>());
		break;
	default:
		break;
	}
}