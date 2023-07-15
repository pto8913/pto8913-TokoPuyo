#pragma once

#include "Sprite.h"

#include <queue>

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMouseAction, float, float);

class DirectX11;

// 
// singleton
// 
class Mouse : public Sprite
{
	friend class Window;
public:
	Mouse(DirectX11& dx, DirectX::XMFLOAT2 inOffset);
	~Mouse();

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	static Mouse& Get(DirectX11& dx);
	
	struct State
	{
		bool left;
		bool right;
		bool middle;
		int x;
		int y;
		int scrollWheelValue;
	};

	class Event
	{
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
			WHEELUP = 21,
			WHEELDOWN = 22,
			WRELEASED = 23,
			WPRESSED = 24,

			/* Other */
			MOVE = 31,
			ENTER = 32,
			LEAVE = 33,
		};
	public:
		Event(ButtonState inState, const Mouse& inMouse) noexcept
		{

		}
	private:
		ButtonState left;
		ButtonState right;
		ButtonState middle;
		int x;
		int y;
	};

	State GetState() const;

	void ResetScrollWheelValue() noexcept;

	void SetWindow(HWND window);
	void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext) override;

	bool IsVisible() const noexcept;
	void SetVisibility(bool inVisible);

	DirectX::XMFLOAT2 GetCursorPosition() const noexcept;
	void SetCursorPosition(DirectX::XMFLOAT2 in);
	void SetCursorPosition2(float x, float y);

	static FOnMouseAction OnClickedLeftDown;
	static FOnMouseAction OnClickedLeftUp;
	
	static FOnMouseAction OnClickedRightDown;
	static FOnMouseAction OnClickedRightUp;

	static FOnMouseAction sOnMouseMove;

public:

	bool IsInWindow() const noexcept;
private:
	// ------------------------------------------------------------------------
	// Main : Event
	// ------------------------------------------------------------------------
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;

	// --------------------------------------
	// Main : LeftButton
	// --------------------------------------
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftRelease(int x, int y) noexcept;

	// --------------------------------------
	// Main : RightButton
	// --------------------------------------
	void OnRightPressed(int x, int y) noexcept;
	void OnRightRelease(int x, int y) noexcept;

	// --------------------------------------
	// Main : WheelButton
	// --------------------------------------
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelPressed(int x, int y) noexcept;
	void OnWheelRelease(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	void TrimBuffer() noexcept;
private:
	bool bIsInWindow = true;
	//std::wstring m_CursorImagePath = ;
	bool bShowCursor = true;

	class Impl;

	std::shared_ptr<Impl> pImpl;

	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> eventBuffer;

	int posX = 0, posY = 0;
	
	int wheelDelta = 0;
};