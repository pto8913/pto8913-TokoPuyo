#pragma once

#include "EventChoiceBase.h"

class S_ChoiceBox;

DECLARE_MULTICAST_DELEGATE(FOnChoice);

class Event_DungeonExit : public EventChoiceBase
{
public:
	Event_DungeonExit(DirectX11& dx);
	virtual ~Event_DungeonExit();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	FOnChoice OnChoiceYes;
	FOnChoice OnChoiceNo;
protected:
	virtual void OnChoiceResult(const FChoiceInfos& inChoiceInfos) override;
};