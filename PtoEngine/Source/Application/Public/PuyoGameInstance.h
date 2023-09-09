#pragma once

#include "Framework/GameInstance.h"
#include "World/WorldTypes.h"

class World;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpenWorld, std::shared_ptr<World>);

class PuyoGameInstance : public GameInstance
{
public:
	PuyoGameInstance() = default;
	virtual ~PuyoGameInstance();

	PuyoGameInstance(const PuyoGameInstance&) = delete;
	PuyoGameInstance& operator=(const PuyoGameInstance&) = delete;

	static PuyoGameInstance& Get();

	void OpenWorld(const EWorldId& id);
	FOnOpenWorld OnOpenWorld;
};