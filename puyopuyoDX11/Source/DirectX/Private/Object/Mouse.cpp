
#include "Object/Mouse.h"

#include "Core/DirectX.h"

#include "Window.h"

FOnMouseAction Mouse::OnClickedLeftDown;
FOnMouseAction Mouse::OnClickedLeftUp;
FOnMouseAction Mouse::OnClickedRightDown;
FOnMouseAction Mouse::OnClickedRightUp;
FOnMouseAction Mouse::sOnMouseMove;

Mouse& Mouse::Get(DirectX11& dx)
{
	static Mouse instance(dx, {0,0});
	return instance;
}

Mouse::Mouse(DirectX11& dx, DirectX::XMFLOAT2 inOffset)
	: Sprite(dx, L"Content/Textures/T_MouseCursor.png", L"Mouse Cursor", { 16.f, 16.f }, inOffset)
{
	//sOnMouseMove.Bind<&Mouse::SetCursorPosition2>(*this);
}
Mouse::~Mouse()
{
}

class Mouse::Impl
{
public:
	Impl(Impl&&) = default;
	Impl& operator=(Impl&&) = default;

	Impl(const Impl&) = delete;
	Impl& operator=(const Impl&) = delete;
private:
	State mState;
};

void Mouse::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pMousePointer;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (GetCursorPos(&pMousePointer))
		{
			OnClickedLeftDown.Broadcast((FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y);
		}
		break;
	case WM_LBUTTONUP:
		if (GetCursorPos(&pMousePointer))
		{
			OnClickedLeftUp.Broadcast((FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y);
		}
		break;
	case WM_RBUTTONDOWN:
		if (GetCursorPos(&pMousePointer))
		{
			OnClickedRightDown.Broadcast((FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y);
		}
		break;
	case WM_RBUTTONUP:
		if (GetCursorPos(&pMousePointer))
		{
			OnClickedRightUp.Broadcast((FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y);
		}
		break;
	case WM_MOUSEMOVE:
		if (GetCursorPos(&pMousePointer))
		{
			SetCursorPosition({ (FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y });
			sOnMouseMove.Broadcast((FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y);
		}
		break;
	default:
		break;
	}
}

void Mouse::ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	if (bShowCursor)
	{
		Sprite::ExecuteTasks(dx, pContext);

		//if (const auto ecode = Window::ProcessMessages())
		//{
		//	switch (*ecode)
		//	{
		//	case WM_LBUTTONDOWN:
		//		OnClickedLeftDown.Broadcast(offset.x, offset.y);
		//		break;
		//	case WM_LBUTTONUP:
		//		OnClickedLeftUp.Broadcast(offset.x, offset.y);
		//		break;
		//	case WM_RBUTTONDOWN:
		//		OnClickedRightDown.Broadcast(offset.x, offset.y);
		//		break;
		//	case WM_RBUTTONUP:
		//		OnClickedRightUp.Broadcast(offset.x, offset.y);
		//		break;
		//	case WM_MOUSEMOVE:
		//		POINT pMousePointer;
		//		if (GetCursorPos(&pMousePointer))
		//		{
		//			SetCursorPosition({ (FLOAT)pMousePointer.x, (FLOAT)pMousePointer.y });
		//		}
		//		OnMouseMove.Broadcast(offset.x, offset.y);
		//		break;
		//	default:
		//		break;
		//	}
		//}
	}
}

bool Mouse::IsVisible() const noexcept
{
	return bShowCursor;
}
void Mouse::SetVisibility(bool inVisible)
{
	bShowCursor = inVisible;
}

DirectX::XMFLOAT2 Mouse::GetCursorPosition() const noexcept
{
	return offset;
}
void Mouse::SetCursorPosition(DirectX::XMFLOAT2 in)
{
	offset = in;
	SetCursorPos(in.x, in.y);
}
void Mouse::SetCursorPosition2(float x, float y)
{
	offset.x = x;
	offset.y = y;
}

bool Mouse::IsInWindow() const noexcept
{
	return bIsInWindow;
}

// ------------------------------------------------------------------------
// Main : Event
// ------------------------------------------------------------------------
void Mouse::OnMouseMove(int x, int y) noexcept
{
	posX = x;
	posY = y;

	SetCursorPosition({ (float)x,(float)y });

	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::MOVE, *this));
	TrimBuffer();
}
void Mouse::OnMouseEnter() noexcept
{
	bIsInWindow = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::ENTER, *this));
	TrimBuffer();
}
void Mouse::OnMouseLeave() noexcept
{
	bIsInWindow = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::LEAVE, *this));
	TrimBuffer();
}

// --------------------------------------
// Main : LeftButton
// --------------------------------------
void Mouse::OnLeftPressed(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::LPRESSED, *this));
	TrimBuffer();
}
void Mouse::OnLeftRelease(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::LRELEASED, *this));
	TrimBuffer();
}

// --------------------------------------
// Main : RightButton
// --------------------------------------
void Mouse::OnRightPressed(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::RPRESSED, *this));
	TrimBuffer();
}
void Mouse::OnRightRelease(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::RRELEASED, *this));
	TrimBuffer();
}

// --------------------------------------
// Main : WheelButton
// --------------------------------------
void Mouse::OnWheelUp(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::WHEELUP, *this));
	TrimBuffer();
}
void Mouse::OnWheelDown(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::WHEELDOWN, *this));
	TrimBuffer();
}
void Mouse::OnWheelPressed(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::WPRESSED, *this));
	TrimBuffer();
}
void Mouse::OnWheelRelease(int x, int y) noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::ButtonState::WRELEASED, *this));
	TrimBuffer();
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

void Mouse::TrimBuffer() noexcept
{
	while (eventBuffer.size() > bufferSize)
	{
		eventBuffer.pop();
	}
}