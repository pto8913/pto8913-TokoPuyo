
#include "Input/Controller.h"
#include "Input/Mouse.h"
//#include "Input/Camera.h"

#include <Windows.h>
#include <wrl/client.h>

Controller::Controller()
{
    pMouse = std::make_shared<Mouse>();
    //m_pCamera = std::make_unique<Camera>(dx, "A", DirectX::XMFLOAT3(0, 0, 5.f), 9.f, PI / 2.f);
}
Controller::~Controller()
{
    //m_pCamera.reset();
    pMouse.reset();
}

// ---------------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------------
void Controller::SetInputMode(DX::FInputMode inMode)
{
    mInputMode = inMode;
}

// -----------------------------------
// Main : Camera
// -----------------------------------
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

