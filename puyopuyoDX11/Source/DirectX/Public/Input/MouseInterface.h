#pragma once

#include "Delegate/Delegate.h"

namespace DX
{
	class MouseEvent
	{
		friend class Mouse;
	public:
		enum class ButtonState
		{
			/* Left */
			LHELD = 1,
			LRELEASED = 2,
			LPRESSED = 3,

			/* Right */
			RHELD = 11,
			RRELEASED = 12,
			RPRESSED = 13,

			/* Wheel */
			WHEELHELD = 21,
			WHEELUP = 22,
			WHEELDOWN = 23,
			WRELEASED = 24,
			WPRESSED = 25,

			/* Other */
			MOVE = 31,
			ENTER = 32,
			LEAVE = 33,
		};
		MouseEvent(ButtonState inState, float posX, float posY) noexcept
			: State(inState),
			x(posX),
			y(posY)
		{}
		ButtonState State;
		float x;
		float y;
	};

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseAction, MouseEvent);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMouseActionDelta, MouseEvent, float);

	struct IMouseInterface
	{
		virtual bool IsVisible() const noexcept = 0;
		virtual void SetVisibility(bool inVisible) = 0;
		virtual bool IsInWindow() const noexcept = 0;

		// ------------------------------------------------------------------------
		// Delegate
		// ------------------------------------------------------------------------
		virtual DX::FOnMouseAction& GetClickedLeftPressed() = 0;
		virtual DX::FOnMouseAction& GetClickedLeftReleased() = 0;
		virtual DX::FOnMouseAction& GetClickedLeftHeld() = 0;

		virtual DX::FOnMouseAction& GetClickedRightPressed() = 0;
		virtual DX::FOnMouseAction& GetClickedRightReleased() = 0;
		virtual DX::FOnMouseAction& GetClickedRightHeld() = 0;

		virtual DX::FOnMouseAction& GetClickedWheelPressed() = 0;
		virtual DX::FOnMouseAction& GetClickedWheelReleased() = 0;
		virtual DX::FOnMouseAction& GetClickedWheelHeld() = 0;

		virtual DX::FOnMouseActionDelta& GetWheelUp() = 0;
		virtual DX::FOnMouseActionDelta& GetWheelDown() = 0;

		// ------------------------------------------------------------------------
		// Main : Event
		// ------------------------------------------------------------------------
		virtual void OnMouseMove(int x, int y) noexcept = 0;
		virtual void OnMouseEnter() noexcept = 0;
		virtual void OnMouseLeave() noexcept = 0;
		// --------------------------------------
		// Main : LeftButton
		// --------------------------------------
		virtual void OnLeftPressed(int x, int y) noexcept = 0;
		virtual void OnLeftReleased(int x, int y) noexcept = 0;
		virtual void OnLeftHeld(int x, int y) noexcept = 0;
		virtual bool IsLeftPressed() const noexcept = 0;

		// --------------------------------------
		// Main : RightButton
		// --------------------------------------
		virtual void OnRightPressed(int x, int y) noexcept = 0;
		virtual void OnRightReleased(int x, int y) noexcept = 0;
		virtual void OnRightHeld(int x, int y) noexcept = 0;
		virtual bool IsRightPressed() const noexcept = 0;

		// --------------------------------------
		// Main : WheelButton
		// --------------------------------------
		virtual void OnWheelUp(int x, int y) noexcept = 0;
		virtual void OnWheelDown(int x, int y) noexcept = 0;
		virtual void OnWheelPressed(int x, int y) noexcept = 0;
		virtual void OnWheelReleased(int x, int y) noexcept = 0;
		virtual void OnWheelHeld(int x, int y) noexcept = 0;
		virtual bool IsWheelPressed() const noexcept = 0;
		virtual void OnWheelDelta(int x, int y, int delta) noexcept = 0;

		// --------------------------------------
		// Main : Raw
		// --------------------------------------
		virtual void OnRawDelta(int dx, int dy) noexcept = 0;
		virtual bool IsRawEnabled() const noexcept = 0;
	};
}