
#include "Framework/PlayerController.h"

#include "Core/DirectX.h"

#include "Input/Mouse.h"

PlayerController::PlayerController(DirectX11& dx)
	: Controller(dx)
{
    SetTickEnabled(true);

    /* Input */
    {
        mKeyBoard = Keyboard(dx.GetHInstance(), dx.GetHWnd());
        mKeyBoard.GetDirectInput()->CreateDevice(GUID_SysMouse, &mDirectMouse, NULL);
        mDirectMouse->SetDataFormat(&c_dfDIMouse);
        mDirectMouse->SetCooperativeLevel(dx.GetHWnd(), DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

        pMouse = std::make_shared<Mouse>();
    }
}
PlayerController::~PlayerController()
{
	pMouse.reset();
	pMouse = nullptr;

	mDirectMouse->Unacquire();
	mDirectMouse = nullptr;

    mDirectMouse = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void PlayerController::Tick(DirectX11& dx, float deltaTime)
{
	InputUpdate(deltaTime);

	ExecuteTasks(dx);
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

// -----------------------------------
// 
// -----------------------------------
void PlayerController::ExecuteTasks(DirectX11& dx)
{
	Controller::ExecuteTasks(dx);
}