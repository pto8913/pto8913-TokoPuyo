
#include "Actor/Event/EventBase.h"

#include "Component/BoxCollision.h"
#include "Component/SpriteComponent.h"

struct FEventSettings : public FActor2DSettings
{
	FEventSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize, Layer::Event)
	{
	}

	FEventSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag, Layer::Event)
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
	mLayer = Layer::EActorLayer::Entities;
	mSortOrder = Layer::Event;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void EventBase::EnterVolume(const float& x, const float& y)
{
	if (OnEnterVolume.IsBound())
	{
		OnEnterVolume.Broadcast(FEventData(FVector(x, y, 0), EventType));
	}
}
void EventBase::LeaveVolume(const float& x, const float& y)
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
	GetSpriteComp()->UpdateTexture(EventSettings.at(inEventType).fileName);
	EventType = inEventType;
}
const EEventId& EventBase::GetEventType() const noexcept
{
	return EventType;
}