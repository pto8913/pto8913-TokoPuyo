
#include "Core/App.h"
#include "Core/AppSettings.h"
#include "Core/DirectX.h"

#include "Engine/Timer.h"
#include "Engine/World.h"

#include "Controller/PlayerController.h"
#include "Input/Keyboard.h"

#include "GameInstance.h"

#include "World/World_SonoTown.h"
#include "World/World_SonoCave.h"

Keyboard::InputAction InputEsc(DIK_ESCAPE);

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    return App().Run();
}

App::App()
    : mWindow()
{
    pDX = std::make_unique<DirectX11>(mWindow.GetHInstance(), mWindow.GetHWnd(), mWindow.GetWidth(), mWindow.GetHeight());

    GameInstance& gameInstance = GameInstance::Get();
    gameInstance.Initialize(*pDX);

    pWorld = std::make_shared<World_SonoTown>();
    pWorld->Init(*pDX);
    pWorld->OnPlayerControllerChanged.Bind<&App::OnPlayerControllerChanged>(*this, "App");
    OnPlayerControllerChanged(pWorld->GetPlayerController());
    pWorld->BeginPlay(*pDX);

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

    pWorld.reset();
    pWorld = nullptr;
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

            pWorld->Tick(*pDX, deltaSec);

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
    if (InputEsc)
    {
        PostQuitMessage(0);
    }
}

// -----------------------------------
// Main : GameMode
// -----------------------------------
void App::OnPlayerControllerChanged(const std::shared_ptr<PlayerController>& pPlayerController)
{
    mWindow.pMouse = pPlayerController->GetMouse();
}
