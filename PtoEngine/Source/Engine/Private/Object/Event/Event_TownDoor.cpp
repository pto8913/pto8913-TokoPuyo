
#include "Object/Event/Event_TownDoor.h"

#include "Engine/World.h"

Event_TownDoor::Event_TownDoor(DirectX11& dx, const ELevelId& inLevelId, const bool& inOpenChoice, const TArray<FChoiceInfos>& inChoiceInfos)
	: EventChoiceBase(dx, EEventId::Exit, inChoiceInfos),
	mOpenLevelId(inLevelId),
	bIsOpenChoice(inOpenChoice)
{
	pDX = &dx;
}
Event_TownDoor::~Event_TownDoor()
{
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void Event_TownDoor::EnterVolume(const int& x, const int& y)
{
	if (bIsOpenChoice)
	{
		EventChoiceBase::EnterVolume(x, y);
	}
	else
	{
		GetWorld()->OpenSubLevel(*pDX, ELevelId::SonoCave);
		//LevelFactory* pLevelFactory = new LevelFactory();
		//pLevelFactory->Create(*pDX, mOpenLevelId);
	}
}
void Event_TownDoor::LeaveVolume(const int& x, const int& y)
{
	if (bIsOpenChoice)
	{
		EventChoiceBase::LeaveVolume(x, y);
	}
}