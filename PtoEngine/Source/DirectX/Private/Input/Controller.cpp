
#include "Input/Controller.h"

Controller::Controller(DirectX11& dx)
{
}
Controller::~Controller()
{
    OnInputModeChanged.ClearBind();
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
}
