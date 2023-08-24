#pragma once

#include "Object/Event/EventChoiceBase.h"

#include "Level/LevelTypes.h"

class Event_TownDoor : public EventChoiceBase
{
public:
	Event_TownDoor(DirectX11& dx, const ELevelId& inLevelId, const bool& inOpenChoice, const TArray<FChoiceInfos>& inChoiceInfos = {});
	virtual ~Event_TownDoor();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void EnterVolume(const int& x, const int& y) override;
	virtual void LeaveVolume(const int& x, const int& y) override;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	bool bIsOpenChoice = false;
	ELevelId mOpenLevelId;
	DirectX11* pDX = nullptr;
};