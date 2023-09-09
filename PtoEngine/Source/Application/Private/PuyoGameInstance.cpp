
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

void PuyoGameInstance::OpenWorld(const EWorldId& id)
{
	switch (id)
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