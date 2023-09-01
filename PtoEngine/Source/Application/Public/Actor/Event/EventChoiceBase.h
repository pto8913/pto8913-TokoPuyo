#pragma once

#include "EventBase.h"

#include "Slate/Message/ChoiceTypes.h"

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
	virtual void EnterVolume(const float& x, const float& y) override;
	virtual void LeaveVolume(const float& x, const float& y) override;

protected:
	void OpenChoiceUI();
	void CloseChoiceUI();
	virtual void OnChoiceResult(const FChoiceInfos& inChoiceInfos) = 0;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	TArray<FChoiceInfos> ChoiceInfos;
	std::shared_ptr<S_ChoiceBox> pChoiceBox = nullptr;
	DirectX11* pDX = nullptr;
};