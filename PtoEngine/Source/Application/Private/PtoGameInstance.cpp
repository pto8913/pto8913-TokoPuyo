
#include "PtoGameInstance.h"

#include "World/World_SonoCave.h"
#include "World/World_SonoTown.h"

PtoGameInstance& PtoGameInstance::Get()
{
    static PtoGameInstance instance;
    return instance;
}

void PtoGameInstance::OpenWorldDelay()
{
     switch (mQueuedOpenWorldID)
     {
     case 0:
         pWorld = std::make_shared<World_SonoCave>();
         break;
     default:
         pWorld = std::make_shared<World_SonoCave>();
         break;
     };

     OnOpenWorld.Broadcast(pWorld);
}