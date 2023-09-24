
#include "App.h"
#include "EngineSettings.h"

#include "Graphics/Core/DirectX.h"

#include "Engine/Timer.h"

#include "Framework/World.h"
#include "Framework/PlayerController.h"
#include "PtoGameInstance.h"

#include "Input/Keyboard.h"

#include "World/World_SonoTown.h"
#include "World/World_SonoCave.h"

#if _DEBUG
#include "UI/DebugUI.h"
#endif

Keyboard::InputAction InputEsc(DIK_ESCAPE);
Keyboard::InputAction InputAt(DIK_AT);

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    return App().Run();
}

App::App()
    : mWindow(EngineSettings::GetWindowSize().x, EngineSettings::GetWindowSize().y)
{
#if _DEBUG
    OutputDebugStringA(("window Aspect : " + EngineSettings::GetWindowAspectRatio().ToString() + "\n").c_str());
    OutputDebugStringA(("window InSize : " + EngineSettings::GetWindowSize().ToString() + "\n").c_str());
#endif
    pDX = std::make_unique<DirectX11>(mWindow.GetHInstance(), mWindow.GetHWnd(), mWindow.GetWidth(), mWindow.GetHeight());

    pGameInstance = &PtoGameInstance::Get();
    pGameInstance->Initialize(*pDX);
    pGameInstance->OnOpenWorld.Bind<&App::OnWorldChanged>(*this, "App");
    pGameInstance->OpenWorld(0);
    
    /* Viewport */
    {
        pViewPort = std::make_unique<ViewPort>((float)mWindow.GetWidth(), (float)mWindow.GetHeight());
        pViewPort->Bind(*pDX);
    }

    pAppTimer = std::make_unique<WorldTimer>();

    bIsInitialized = true;
}
App::~App()
{
    pDX.reset();
    pDX = nullptr;

    pViewPort.reset();
    pViewPort = nullptr;

    pAppTimer.reset();
    pAppTimer = nullptr;

    pGameInstance = nullptr;

#if _DEBUG
    pDebugUI = nullptr;
#endif
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
int App::Run()
{
    while (true)
    {
        if (bIsInitialized)
        {
            if (const auto ecode = Window::ProcessMessages())
            {
                CoUninitialize();

                return *ecode;
            }

            pDX->BeginFrame();

            const auto deltaSec = pAppTimer->GetDelta() * appTimerSpeed;

            InputUpdate(*pDX);

            pGameInstance->Tick(*pDX, deltaSec);

            HRESULT result = pDX->EndFrame();
            if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
            {
                //OnDeviceLost();
            }
            else
            {
                if (FAILED(result))
                {
                    assert(false);
                }
            }
        }
    }
}
void App::InputUpdate(DirectX11& dx)
{
#if _DEBUG
    if (InputAt)
    {
        bOpenDebugUI = !bOpenDebugUI;
        if (bOpenDebugUI)
        {
            OpenDebugUI();
        }
        else
        {
            CloseDebugUI();
        }
    }
#endif
    if (InputEsc)
    {
        PostQuitMessage(0);
    }
}

// -----------------------------------
// Main : GameMode
// -----------------------------------
void App::OnPlayerControllerChanged(PlayerController* pPlayerController)
{
    mWindow.pMouse = pPlayerController->GetMouse();
}
void App::OnWorldChanged(World* NewWorld)
{
    NewWorld->Init(*pDX);
    NewWorld->OnPlayerControllerChanged.Bind<&App::OnPlayerControllerChanged>(*this, "App");
    OnPlayerControllerChanged(NewWorld->GetPlayerController());
    NewWorld->BeginPlay(*pDX);

#if _DEBUG
    if (IsValid(pDebugUI))
    {
        pDebugUI = nullptr;
    }
    if (bOpenDebugUI)
    {
        OpenDebugUI();
    }
#endif
}

// -----------------------------------
// Main : Debug
// -----------------------------------
void App::OpenDebugUI()
{
#if _DEBUG
    bOpenDebugUI = true;

    if (!IsValid(pDebugUI))
    {
        pDebugUI = pGameInstance->GetWorld()->CreateWidget<DebugUI>(
            nullptr,
            *pDX,
            nullptr
        );
    }
    pDebugUI->AddToViewport();
#endif
}
void App::CloseDebugUI()
{
#if _DEBUG
    bOpenDebugUI = false;
    if (IsValid(pDebugUI))
    {
        pDebugUI->RemoveFromParent();
    }
#endif
}
