
#include "Controller/PlayerController.h"

#include "Object/Character/Player.h"

#include "Core/AppSettings.h"

#include "Input/Mouse.h"

#include "UI/HUD.h"

PlayerController::PlayerController(DirectX11& dx)
	: Controller(dx)
{
    /* Input */
    {
        mKeyBoard = Keyboard(dx.GetHInstance(), dx.GetHWnd());
        mKeyBoard.GetDirectInput()->CreateDevice(GUID_SysMouse, &mDirectMouse, NULL);
        mDirectMouse->SetDataFormat(&c_dfDIMouse);
        mDirectMouse->SetCooperativeLevel(dx.GetHWnd(), DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

        pMouse = std::make_shared<Mouse>();
    }

	pPlayer = std::make_shared<Player>(dx);

	pHUD = std::make_shared<HUD>(dx, pMouse.get(), (int)AppSettings::windowSize.x, (int)AppSettings::windowSize.y);
	pHUD->AddToViewport();
}
PlayerController::~PlayerController()
{
	pMouse.reset();
	pMouse = nullptr;

	mDirectMouse->Unacquire();
	mDirectMouse = nullptr;

    pPlayer.reset();
    pPlayer = nullptr;
    
    pHUD.reset();
    pHUD = nullptr;

    mDirectMouse = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void PlayerController::Tick(DirectX11& dx, float deltaTime)
{
	InputUpdate(deltaTime);

	pHUD->ExecuteTasks(dx);

	ExecuteTasks(dx);
	pPlayer->DoFrame(dx, deltaTime);
}

// -----------------------------------
// Main : Input
// -----------------------------------
void PlayerController::InputUpdate(float deltaTime)
{
    mKeyBoard.Update();

    if (pMouse->IsVisible())
    {
        switch (mInputMode)
        {
        case DX::FInputMode::GameOnly:
            UpdateDirectInputMouse();
            break;
        case DX::FInputMode::UIOnly:
            break;
        case DX::FInputMode::GameAndUI:
            if (pMouse->IsLeftPressed() || pMouse->IsRightPressed())
            {
                UpdateDirectInputMouse();
            }
            else
            {
                mDirectMouse->Unacquire();
            }
            break;
        default:
            break;
        }
    }
}
void PlayerController::SetInputMode(DX::FInputMode inMode)
{
    Controller::SetInputMode(inMode);

    switch (inMode)
    {
    case DX::FInputMode::GameOnly:
        mDirectMouse->Acquire();
        break;
    case DX::FInputMode::UIOnly:
        mDirectMouse->Unacquire();
        break;
    case DX::FInputMode::GameAndUI:
        mDirectMouse->Unacquire();
        break;
    default:
        break;
    }
}

// ----------------------------------
// Main : Mouse
// ----------------------------------
LONG PlayerController::GetMouseDeltaX() const noexcept
{
    return mMouseCurrState.lX;
}
LONG PlayerController::GetMouseDeltaY() const noexcept
{
    return mMouseCurrState.lY;
}

void PlayerController::UpdateDirectInputMouse()
{
    mDirectMouse->Acquire();
    mDirectMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseCurrState);

    if ((mMouseCurrState.lX != mMouseLastState.lX) || (mMouseCurrState.lY != mMouseLastState.lY))
    {
        mMouseLastState = mMouseCurrState;
    }
}

// -----------------------------------
// Main : Util
// -----------------------------------
DX::IMouseInterface* PlayerController::GetMouse()
{
    return pMouse.get();
}
std::shared_ptr<Player>& PlayerController::GetPlayer()
{
	return pPlayer;
}
std::shared_ptr<HUD>& PlayerController::GetHUD()
{
	return pHUD;
}

// -----------------------------------
// 
// -----------------------------------
void PlayerController::ExecuteTasks(DirectX11& dx)
{
	Controller::ExecuteTasks(dx);
}