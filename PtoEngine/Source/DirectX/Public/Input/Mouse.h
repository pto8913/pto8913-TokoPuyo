#pragma once

#include "Engine/Delegate.h"
#include <queue>
#include <optional>

#include "Input/MouseInterface.h"


class DirectX11;

// 
// singleton
// 
class Mouse : public DX::IMouseInterface
{
	friend class Window;
public:
	Mouse();
	~Mouse();

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	static DX::IMouseInterface& GetInterface()
	{
		static Mouse instance;
		return instance;
	}

	struct Raw
	{
		int x;
		int y;
	};

	// ------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------
	virtual bool IsVisible() const noexcept override;
	virtual void SetVisibility(bool inVisible) override;

	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	void SetCursorPosition(int x, int y);

	// --------------------------------------
	// Main : Delegate
	// --------------------------------------
	virtual DX::FOnMouseAction& GetMouseMove() override;

	virtual DX::FOnMouseAction& GetClickedLeftPressed() override;
	virtual DX::FOnMouseAction& GetClickedLeftReleased() override;
	virtual DX::FOnMouseAction& GetClickedLeftHeld() override;

	virtual DX::FOnMouseAction& GetClickedRightPressed() override;
	virtual DX::FOnMouseAction& GetClickedRightReleased() override;
	virtual DX::FOnMouseAction& GetClickedRightHeld() override;

	virtual DX::FOnMouseAction& GetClickedWheelPressed() override;
	virtual DX::FOnMouseAction& GetClickedWheelReleased() override;
	virtual DX::FOnMouseAction& GetClickedWheelHeld() override;

	virtual DX::FOnMouseActionDelta& GetWheelUp() override;
	virtual DX::FOnMouseActionDelta& GetWheelDown() override;
private:
	// --------------------------------------
	// Main : Event
	// --------------------------------------
	virtual void OnMouseMove(int x, int y) noexcept override;
	virtual void OnMouseEnter() noexcept override;
	virtual void OnMouseLeave() noexcept override;
public:
	virtual bool IsInWindow() const noexcept override;
	std::optional<DX::MouseEvent> Read() noexcept;
private:
	// --------------------------------------
	// Main : LeftButton
	// --------------------------------------
	virtual void OnLeftPressed(int x, int y) noexcept override;
	virtual void OnLeftReleased(int x, int y) noexcept override;
	virtual void OnLeftHeld(int x, int y) noexcept override;
public:
	virtual bool IsLeftPressed() const noexcept override;
private:
	// --------------------------------------
	// Main : RightButton
	// --------------------------------------
	virtual void OnRightPressed(int x, int y) noexcept override;
	virtual void OnRightReleased(int x, int y) noexcept override;
	virtual void OnRightHeld(int x, int y) noexcept override;
public:
	virtual bool IsRightPressed() const noexcept override;
private:
	// --------------------------------------
	// Main : WheelButton
	// --------------------------------------
	virtual void OnWheelUp(int x, int y) noexcept override;
	virtual void OnWheelDown(int x, int y) noexcept override;
	virtual void OnWheelPressed(int x, int y) noexcept override;
	virtual void OnWheelReleased(int x, int y) noexcept override;
	virtual void OnWheelHeld(int x, int y) noexcept override;
	virtual void OnWheelDelta(int x, int y, int delta) noexcept override;
public:
	virtual bool IsWheelPressed() const noexcept override;
private:
	// --------------------------------------
	// Main : Raw
	// --------------------------------------
	void OnRawDelta(int dx, int dy) noexcept;
public:
	virtual bool IsRawEnabled() const noexcept override;
	void SetRawEnabled(bool inEnable) noexcept;
	std::optional<Raw> ReadRawDelta() noexcept;
private:
	void TrimBuffer() noexcept;
private:
	// ------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------
	bool bIsInWindow = true;
	bool bShowCursor = true;
	bool bIsLeftPressed = false;
	bool bIsRightPressed = false;
	bool bIsWheelPressed = false;

	static constexpr unsigned int bufferSize = 16u;
	std::queue<DX::MouseEvent> eventBuffer;
	std::queue<Raw> rawEventBuffer;

	int posX = 0;
	int posY = 0;
	int posXLast = 0;
	int posYLast = 0;
	
	int wheelDelta = 0;

	bool bEnableRawInput = false;

	// --------------------------------------
	// State : DInput
	// --------------------------------------

	// --------------------------------------
	// State : Delegate
	// --------------------------------------
	DX::FOnMouseAction OnClickedLeftPressed;
	DX::FOnMouseAction OnClickedLeftReleased;
	DX::FOnMouseAction OnClickedLeftHeld;

	DX::FOnMouseAction OnClickedRightPressed;
	DX::FOnMouseAction OnClickedRightReleased;
	DX::FOnMouseAction OnClickedRightHeld;

	DX::FOnMouseAction OnClickedWheelPressed;
	DX::FOnMouseAction OnClickedWheelReleased;
	DX::FOnMouseAction OnClickedWheelHeld;

	DX::FOnMouseAction OnMouseMoving;

	DX::FOnMouseActionDelta OnWheelUpDelta;
	DX::FOnMouseActionDelta OnWheelDownDelta;
};