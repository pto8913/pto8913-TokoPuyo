#pragma once

#include "Input/MouseInterface.h"
#include "Input/CameraInterface.h"

class DirectX11;

namespace DX
{
	enum class FInputMode
	{
		GameOnly, /* Only the game process receives input events from keyboard or mouse, etc... */
		UIOnly, /* Only the UI receives input events from the Keyboard or mouse, etc... */
		GameAndUI, /* if you want to move camera during open ui. */
	};

	struct IControllerInterface
	{
		virtual void SetInputMode(FInputMode inMode) = 0;
		virtual FInputMode GetInputMode() const noexcept = 0;
		virtual void ExecuteTasks(DirectX11& dx) = 0;

		// -----------------------------------
		// Main : Camera
		// -----------------------------------
		virtual DX::ICameraInterface* GetCameraInterface() = 0;
		virtual void SetCameraEnabled(bool inEnabled) = 0;
		virtual bool IsEnableCamera() const noexcept = 0;

		// -----------------------------------
		// Main : Mouse
		// -----------------------------------
		virtual DX::IMouseInterface* GetMouseInterface() = 0;

		virtual void SetMouseEnabled(bool inEnabled) = 0;
		virtual bool IsEnableMouse() const noexcept = 0;

		virtual long GetMouseDeltaX() const noexcept = 0;
		virtual long GetMouseDeltaY() const noexcept = 0;
	};
}