
#include "Actor/Event/Event_TownDoor.h"

#include "Framework/World.h"
#include "Level/LevelFactory.h"

Event_TownDoor::Event_TownDoor(DirectX11& dx, const ELevelId& inLevelId, const bool& inOpenChoice, const TArray<FChoiceInfos>& inChoiceInfos)
	: EventChoiceBase(dx, EEventId::Exit, inChoiceInfos),
	mOpenLevelId(inLevelId),
	bIsOpenChoice(inOpenChoice)
{
}
Event_TownDoor::~Event_TownDoor()
{
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void Event_TownDoor::EnterVolume(const float& x, const float& y)
{
	if (bIsOpenChoice)
	{
		EventChoiceBase::EnterVolume(x, y);
	}
	else
	{
		LevelFactory* pLevelFactory = new LevelFactory();
		GetWorld()->OpenSubLevel(*pDX, pLevelFactory->Create(*pDX, mOpenLevelId));
		delete pLevelFactory;
	}
}
void Event_TownDoor::LeaveVolume(const float& x, const float& y)
{
	if (bIsOpenChoice)
	{
		EventChoiceBase::LeaveVolume(x, y);
	}
}