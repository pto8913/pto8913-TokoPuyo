
#include "PtoGameInstance.h"

#include "World/World_Title.h"
#include "World/World_TokoPuyo.h"
#include "World/WorldTypes.h"

#include "Engine/Audio.h"

#include "GameSettings.h"

PtoGameInstance& PtoGameInstance::Get()
{
    static PtoGameInstance instance;
    return instance;
}

void PtoGameInstance::Initialize(DirectX11& dx)
{
    GameInstance::Initialize(dx);

    pAudio_BGM = std::make_shared<Audio>(L"Content/Sounds/Puyo_BGM.wav");
    if (pAudio_BGM)
    {
        pAudio_BGM->SetVolume(0.25f);
        pAudio_BGM->SetLoop(true);
        pAudio_BGM->Play();
    }
    OnAudioVolumeChanged(GameSettings::GetAudioVolume());
}

// -------------------------------------------------------
// Main
// -------------------------------------------------------
void PtoGameInstance::SetPause(bool inPause)
{
    if (pAudio_BGM != nullptr)
    {
        SetBGM(!inPause);
        if (inPause)
        {
        }
        else
        {
        }
    }
}

// -----------------------------
// Main : Audio
// -----------------------------
void PtoGameInstance::SetBGM(bool inPlay)
{
    if (inPlay)
    {
        if (!pAudio_BGM->IsPlaying())
        {
            pAudio_BGM->Play();
        }
    }
    else
    {
        if (pAudio_BGM->IsPlaying())
        {
            pAudio_BGM->Stop();
        }
    }
}
void PtoGameInstance::OnAudioVolumeChanged(float inValue)
{
    pAudio_BGM->SetVolume(pAudio_BGM->GetDefaultVolume() * inValue / 10.f);
}

// -----------------------------
// Main : World
// -----------------------------
void PtoGameInstance::OpenWorldDelay()
{
    pWorld.reset();
    pWorld = nullptr;
 
    const auto OpenWorldID = static_cast<EWorldId>(mQueuedOpenWorldID);
    switch (OpenWorldID)
    {
    case EWorldId::Title:
        pWorld = std::make_unique<World_Title>();
        break;
    default:
        pWorld = std::make_unique<World_TokoPuyo>();
        break;
    };

    OnOpenWorld.Broadcast(pWorld.get());
}
