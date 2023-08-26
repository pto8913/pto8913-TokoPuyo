
#include "Object/Event/EventBase.h"
#include "Component/BoxCollision.h"

struct FEventSettings : public FActor2DSettings
{
	FEventSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize)
	{
	}

	FEventSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag)
	{
	}
};

const std::map<EEventId, FEventSettings> EventSettings =
{
	{EEventId::Enter, FEventSettings(L"Content/Textures/T_Transparent.png", L"Enter")},
	{EEventId::Exit, FEventSettings(L"Content/Textures/T_Dungeon_Exit.png", L"Exit")},
	{EEventId::EnterRoom, FEventSettings(L"Content/Textures/T_Transparent.png", L"EnterRoom")},
	{EEventId::Block, FEventSettings(L"Content/Textures/T_Transparent.png", L"Block")},
};

EventBase::EventBase(DirectX11& dx, const EEventId& inEventType)
	: Actor2D(
		dx, 
		EventSettings.at(inEventType),
		1.f
	),
	EventType(inEventType)
{
	AddComponent<BoxCollision>("collision", this);
	mSortOrder = Layer::Event;
	mLayer = EActor2DLayer::Entities;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void EventBase::EnterVolume(const int& x, const int& y)
{
	if (OnEnterVolume.IsBound())
	{
		OnEnterVolume.Broadcast(FEventData(FVector(x, y, 0), EventType));
	}
}
void EventBase::LeaveVolume(const int& x, const int& y)
{
	if (OnLeaveVolume.IsBound())
	{
		OnLeaveVolume.Broadcast(FEventData(FVector(x, y, 0), EventType));
	}
}

FOnSendEvent& EventBase::GetOnEnterVolume()
{
	return OnEnterVolume;
}
FOnSendEvent& EventBase::GetOnLeaveVolume()
{
	return OnLeaveVolume;
}

void EventBase::SetEventType(const EEventId& inEventType)
{
	UpdateTexture(EventSettings.at(inEventType).fileName);
	EventType = inEventType;
}
const EEventId& EventBase::GetEventType() const noexcept
{
	return EventType;
}