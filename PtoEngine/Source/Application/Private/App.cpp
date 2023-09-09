
#include "App.h"
#include "EngineSettings.h"
#include "PuyoGameInstance.h"

#include "Core/DirectX.h"

#include "Engine/Timer.h"

#include "Framework/World.h"
#include "Framework/PlayerController.h"

#include "Input/Keyboard.h"

Keyboard::InputAction InputEsc(DIK_ESCAPE);

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

    PuyoGameInstance& gameInstance = PuyoGameInstance::Get();
    gameInstance.Initialize(*pDX);
    gameInstance.OnOpenWorld.Bind<&App::OnWorldChanged>(*this, "App");
    gameInstance.OpenWorld(EWorldId::Title);

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

            if (pWorld != nullptr)
            {
                pWorld->Tick(*pDX, deltaSec);
            }

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

void App::OnPlayerControllerChanged(const std::shared_ptr<PlayerController>& pPlayerController)
{
    mWindow.pMouse = pPlayerController->GetMouse();
}
void App::OnWorldChanged(std::shared_ptr<World> NewWorld)
{
    if (pWorld != nullptr)
    {
        pWorld.reset();
        pWorld = nullptr;
    }
    OutputDebugStringA("OnWorldChanged");
    pWorld = std::move(NewWorld);
    pWorld->Init(*pDX);
    pWorld->OnPlayerControllerChanged.Bind<&App::OnPlayerControllerChanged>(*this, "App");
    OnPlayerControllerChanged(pWorld->GetPlayerController());
    pWorld->BeginPlay(*pDX);
}
