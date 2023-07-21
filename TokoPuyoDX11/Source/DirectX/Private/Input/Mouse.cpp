
#include "Input/Mouse.h"
#include <Windows.h>
#include <wrl/client.h>

Mouse::Mouse()
{
}
Mouse::~Mouse()
{
}

bool Mouse::IsVisible() const noexcept
{
	return bShowCursor;
}
void Mouse::SetVisibility(bool inVisible)
{
	bShowCursor = inVisible;
	if (bShowCursor)
	{
		while (::ShowCursor(TRUE) < 0);
	}
	else
	{
		while (::ShowCursor(FALSE) >= 0);
	}
}

int Mouse::GetPosX() const noexcept
{
	return posX;
}
int Mouse::GetPosY() const noexcept
{
	return posY;
}
void Mouse::SetCursorPosition(int x, int y)
{
	posX = x;
	posY = y;
}

// ------------------------------------------------------------------------
// Delegate
// ------------------------------------------------------------------------
DX::FOnMouseAction& Mouse::GetMouseMove()
{
	return OnMouseMoving;
}
DX::FOnMouseAction& Mouse::GetClickedLeftPressed()
{
	return OnClickedLeftPressed;
}
DX::FOnMouseAction& Mouse::GetClickedLeftReleased()
{
	return OnClickedLeftReleased;
}
DX::FOnMouseAction& Mouse::GetClickedLeftHeld()
{
	return OnClickedLeftHeld;
}

DX::FOnMouseAction& Mouse::GetClickedRightPressed()
{
	return OnClickedRightPressed;
}
DX::FOnMouseAction& Mouse::GetClickedRightReleased()
{
	return OnClickedRightReleased;
}
DX::FOnMouseAction& Mouse::GetClickedRightHeld()
{
	return OnClickedRightHeld;
}

DX::FOnMouseAction& Mouse::GetClickedWheelPressed()
{
	return OnClickedWheelPressed;
}
DX::FOnMouseAction& Mouse::GetClickedWheelReleased()
{
	return OnClickedWheelReleased;
}
DX::FOnMouseAction& Mouse::GetClickedWheelHeld()
{
	return OnClickedWheelHeld;
}

DX::FOnMouseActionDelta& Mouse::GetWheelUp()
{
	return OnWheelUpDelta;
}
DX::FOnMouseActionDelta& Mouse::GetWheelDown()
{
	return OnWheelDownDelta;
}

// ------------------------------------------------------------------------
// Main : Event
// ------------------------------------------------------------------------
void Mouse::OnMouseMove(int x, int y) noexcept
{
	SetCursorPosition(x, y);
	DX::MouseEvent Event(DX::MouseEvent::ButtonState::MOVE, x, y);
	eventBuffer.push(Event);
	OnMouseMoving.Broadcast(Event);
	TrimBuffer();
}
void Mouse::OnMouseEnter() noexcept
{
	bIsInWindow = true;
	eventBuffer.push(DX::MouseEvent(DX::MouseEvent::ButtonState::ENTER, posX, posY));
	TrimBuffer();
}
void Mouse::OnMouseLeave() noexcept
{
	bIsInWindow = false;
	eventBuffer.push(DX::MouseEvent(DX::MouseEvent::ButtonState::LEAVE, posX, posY));
	TrimBuffer();
}

bool Mouse::IsInWindow() const noexcept
{
	return bIsInWindow;
}
std::optional<DX::MouseEvent> Mouse::Read() noexcept
{
	if (eventBuffer.size() > 0u)
	{
		DX::MouseEvent out = eventBuffer.front();
		eventBuffer.pop();
		return out;
	}
	return {};
}

// --------------------------------------
// Main : LeftButton
// --------------------------------------
void Mouse::OnLeftPressed(int x, int y) noexcept
{
	bIsLeftPressed = true;

	DX::MouseEvent Event(DX::MouseEvent::ButtonState::LPRESSED, x, y);
	OnClickedLeftPressed.Broadcast(Event);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnLeftReleased(int x, int y) noexcept
{
	bIsLeftPressed = false;

	DX::MouseEvent Event(DX::MouseEvent::ButtonState::LRELEASED, x, y);
	OnClickedLeftReleased.Broadcast(Event);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnLeftHeld(int x, int y) noexcept
{
	if (bIsLeftPressed)
	{
		DX::MouseEvent Event(DX::MouseEvent::ButtonState::LHELD, x, y);
		OnClickedLeftHeld.Broadcast(Event);
	}
}
bool Mouse::IsLeftPressed() const noexcept
{
	return bIsLeftPressed;
}

// --------------------------------------
// Main : RightButton
// --------------------------------------
void Mouse::OnRightPressed(int x, int y) noexcept
{
	bIsRightPressed = true;

	DX::MouseEvent Event(DX::MouseEvent::ButtonState::RPRESSED, x, y);
	OnClickedRightPressed.Broadcast(Event);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnRightReleased(int x, int y) noexcept
{
	bIsRightPressed = false;

	DX::MouseEvent Event(DX::MouseEvent::ButtonState::RRELEASED, x, y);
	OnClickedRightReleased.Broadcast(Event);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnRightHeld(int x, int y) noexcept
{
	if (bIsRightPressed)
	{
		DX::MouseEvent Event(DX::MouseEvent::ButtonState::RHELD, x, y);
		OnClickedRightHeld.Broadcast(Event);
	}
}
bool Mouse::IsRightPressed() const noexcept
{
	return bIsRightPressed;
}

// --------------------------------------
// Main : WheelButton
// --------------------------------------
void Mouse::OnWheelUp(int x, int y) noexcept
{
	DX::MouseEvent Event(DX::MouseEvent::ButtonState::WHEELUP, x, y);
	OnWheelUpDelta.Broadcast(Event, wheelDelta);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnWheelDown(int x, int y) noexcept
{
	DX::MouseEvent Event(DX::MouseEvent::ButtonState::WHEELDOWN, x, y);
	OnWheelDownDelta.Broadcast(Event, wheelDelta);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnWheelPressed(int x, int y) noexcept
{
	bIsWheelPressed = true;
	DX::MouseEvent Event(DX::MouseEvent::ButtonState::WPRESSED, x, y);
	OnClickedWheelPressed.Broadcast(Event);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnWheelReleased(int x, int y) noexcept
{
	bIsWheelPressed = false;
	DX::MouseEvent Event(DX::MouseEvent::ButtonState::WRELEASED, x, y);
	OnClickedWheelReleased.Broadcast(Event);
	eventBuffer.push(Event);
	TrimBuffer();
}
void Mouse::OnWheelHeld(int x, int y) noexcept
{
	if (bIsWheelPressed)
	{
		DX::MouseEvent Event(DX::MouseEvent::ButtonState::WHEELHELD, x, y);
		OnClickedWheelHeld.Broadcast(Event);
	}
}
bool Mouse::IsWheelPressed() const noexcept
{
	return bIsWheelPressed;
}
void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDelta += delta;
	while (wheelDelta >= WHEEL_DELTA)
	{
		wheelDelta -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDelta <= -WHEEL_DELTA)
	{
		wheelDelta += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}


// --------------------------------------
// Main : Raw
// --------------------------------------
void Mouse::OnRawDelta(int dx, int dy) noexcept
{
	rawEventBuffer.push({ dx, dy });
	TrimBuffer();
}
bool Mouse::IsRawEnabled() const noexcept
{
	return bEnableRawInput;
}
void Mouse::SetRawEnabled(bool inEnable) noexcept
{
	bEnableRawInput = inEnable;
}
std::optional<Mouse::Raw> Mouse::ReadRawDelta() noexcept
{
	if (!rawEventBuffer.empty())
	{
		const Mouse::Raw out = rawEventBuffer.front();
		rawEventBuffer.pop();
		return out;
	}
	return {};
}

void Mouse::TrimBuffer() noexcept
{
	while (eventBuffer.size() > bufferSize)
	{
		eventBuffer.pop();
	}
}