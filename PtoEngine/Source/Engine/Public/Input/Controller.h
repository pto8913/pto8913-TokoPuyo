#pragma once

#include "Input/ControllerInterface.h"
#include <memory>

#include "Engine/Delegate.h"

class DirectX11;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputModeChanged, const DX::FInputMode&);

class Controller : public DX::IControllerInterface
{
public:
	Controller(DirectX11& dx);
	virtual ~Controller();

	// ---------------------------------------------------------------------------------
	// Main
	// ---------------------------------------------------------------------------------
	virtual void SetInputMode(DX::FInputMode inMode) override;
	virtual DX::FInputMode GetInputMode() const noexcept override;

	virtual void ExecuteTasks(DirectX11& dx) override;

	FOnInputModeChanged OnInputModeChanged;
private:
	// ---------------------------------------------------------------------------------
	// State
	// ---------------------------------------------------------------------------------
	int mId = 0;
	DX::FInputMode mInputMode = DX::FInputMode::GameOnly;
};