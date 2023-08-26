#pragma once

#include "Object/Actor2D.h"
#include "Level/Layer/EventTypes.h"

class DirectX11;

class EventBase : public Actor2D
{
public:
	EventBase(DirectX11& dx, const EEventId& inEventType);

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void EnterVolume(const int& x, const int& y);
	virtual void LeaveVolume(const int& x, const int& y);

	FOnSendEvent& GetOnEnterVolume();
	FOnSendEvent& GetOnLeaveVolume();

	void SetEventType(const EEventId& inEventLayer);
	const EEventId& GetEventType() const noexcept;

private:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

	EEventId EventType;

	FOnSendEvent OnEnterVolume;
	FOnSendEvent OnLeaveVolume;
};