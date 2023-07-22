
#include "Input/Controller.h"
#include "Input/Mouse.h"
#include "Input/Camera.h"

#include <Windows.h>
#include <wrl/client.h>

Controller::Controller(DirectX11& dx, HINSTANCE hInstance, HWND hWnd)
{
    m_keyBoard = Keyboard(hInstance, hWnd);
    m_keyBoard.GetDirectInput()->CreateDevice(GUID_SysMouse, &mDirectMouse, NULL);
    mDirectMouse->SetDataFormat(&c_dfDIMouse);
    mDirectMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

    pMouse = std::make_shared<Mouse>();

    pCamera = std::make_unique<Camera>(dx, std::to_string(mId), DirectX::XMFLOAT3(0, 0, 5.f), 9.f, PI / 2.f);
}
Controller::~Controller()
{
    pCamera.reset();
    pMouse.reset();

    mDirectMouse->Unacquire();
}

// ---------------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------------
void Controller::SetInputMode(DX::FInputMode inMode)
{
    mInputMode = inMode;
    switch (mInputMode)
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
DX::FInputMode Controller::GetInputMode() const noexcept
{
    return mInputMode;
}

void Controller::ExecuteTasks(DirectX11& dx)
{
    m_keyBoard.Update();

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

    pCamera->ExecuteTasks(dx);
}

// -----------------------------------
// Main : Camera
// -----------------------------------
DX::ICameraInterface* Controller::GetCameraInterface()
{
    return pCamera.get();
}
void Controller::SetCameraEnabled(bool inEnabled)
{
    bEnableCamera = inEnabled;
}
bool Controller::IsEnableCamera() const noexcept
{
    return bEnableCamera;
}

// ----------------------------------
// Main : Mouse
// ----------------------------------
DX::IMouseInterface* Controller::GetMouseInterface()
{
    return pMouse.get();
}
void Controller::SetMouseEnabled(bool inEnabled)
{
    if (inEnabled)
    {
        pMouse->SetVisibility(true);
    }
    else
    {
        pMouse->SetVisibility(false);
    }
}
bool Controller::IsEnableMouse() const noexcept
{
    return pMouse->IsVisible();
}

LONG Controller::GetMouseDeltaX() const noexcept
{
    return mMouseCurrState.lX;
}
LONG Controller::GetMouseDeltaY() const noexcept
{
    return mMouseCurrState.lY;
}

void Controller::UpdateDirectInputMouse()
{
    mDirectMouse->Acquire();
    mDirectMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseCurrState);

    if ((mMouseCurrState.lX != mMouseLastState.lX) || (mMouseCurrState.lY != mMouseLastState.lY))
    {
        mMouseLastState = mMouseCurrState;
    }
}