
#include "PtoGameInstance.h"

#include "World/World_Title.h"
#include "World/World_TokoPuyo.h"

#include "World/WorldTypes.h"

PtoGameInstance& PtoGameInstance::Get()
{
    static PtoGameInstance instance;
    return instance;
}

void PtoGameInstance::OpenWorldDelay()
{
    OutputDebugStringA("OpenWorldDelay Break");
    pWorld.reset();
    pWorld = nullptr;
 
    const auto OpenWorldID = static_cast<EWorldId>(mQueuedOpenWorldID);
    switch (OpenWorldID)
    {
    case EWorldId::Title:
        pWorld = std::make_shared<World_Title>();
        break;
    default:
        pWorld = std::make_shared<World_TokoPuyo>();
        break;
    };

    OnOpenWorld.Broadcast(pWorld);
}