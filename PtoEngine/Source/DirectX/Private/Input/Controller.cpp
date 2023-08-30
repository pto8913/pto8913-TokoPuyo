
#include "Input/Controller.h"
#include "Input/Camera.h"

Controller::Controller(DirectX11& dx)
{
    pCamera = std::make_unique<Camera>(dx, std::to_string(mId), DirectX::XMFLOAT3(0, 0, 5.f), 9.f, PI / 2.f);
}
Controller::~Controller()
{
    OnInputModeChanged.ClearBind();

    pCamera.reset();
    pCamera = nullptr;
}

// ---------------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------------
void Controller::SetInputMode(DX::FInputMode inMode)
{
    mInputMode = inMode;
    OnInputModeChanged.Broadcast(mInputMode);
}
DX::FInputMode Controller::GetInputMode() const noexcept
{
    return mInputMode;
}

void Controller::ExecuteTasks(DirectX11& dx)
{
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