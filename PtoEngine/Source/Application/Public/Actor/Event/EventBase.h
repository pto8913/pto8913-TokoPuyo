#pragma once

#include "Actor/Actor2D.h"
#include "EventTypes.h"

class DirectX11;

class EventBase : public Actor2D
{
public:
	EventBase(DirectX11& dx, const EEventId& inEventType);

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void EnterVolume(const float& x, const float& y);
	virtual void LeaveVolume(const float& x, const float& y);

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