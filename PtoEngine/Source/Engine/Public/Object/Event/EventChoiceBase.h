#pragma once

#include "EventBase.h"

#include "UI/Message/ChoiceTypes.h"

#include "Algorithm/Array.h"

class S_ChoiceBox;

/*
* 
*/
class EventChoiceBase : public EventBase
{
public:
	EventChoiceBase(DirectX11& dx, const EEventId& inEventType, const TArray<FChoiceInfos>& inChoiceInfos = {});
	virtual ~EventChoiceBase();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void EnterVolume(const int& x, const int& y) override;
	virtual void LeaveVolume(const int& x, const int& y) override;

protected:
	void OpenChoiceUI();
	void CloseChoiceUI();
	virtual void OnChoiceResult(const FChoiceInfos& inChoiceInfos) = 0;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	TArray<FChoiceInfos> ChoiceInfos;
	std::shared_ptr<S_ChoiceBox> pChoiceBox = nullptr;
};