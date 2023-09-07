
#include "Actor/Event/EventChoiceBase.h"

#include "Slate/Message/S_ChoiceBox.h"

#include "GameSettings.h"

#include "Framework/World.h"
#include "UI/HUD.h"

EventChoiceBase::EventChoiceBase(DirectX11& dx, const EEventId& inEventType, const TArray<FChoiceInfos>& inChoiceInfos)
	: EventBase(dx, inEventType)
{
	pDX = &dx;
	if (inChoiceInfos.Size() > 0)
	{
		ChoiceInfos = inChoiceInfos;
	}
}
EventChoiceBase::~EventChoiceBase()
{
	CloseChoiceUI();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void EventChoiceBase::EnterVolume(const float& x, const float& y)
{
	OpenChoiceUI();
}
void EventChoiceBase::LeaveVolume(const float& x, const float& y)
{
	CloseChoiceUI();
}

void EventChoiceBase::OpenChoiceUI()
{
	auto& pHUD = GetWorld()->GetHUD();
	pChoiceBox = std::make_shared<S_ChoiceBox>(pHUD->GetRt2D());
	pChoiceBox->GetOnChoiceResult().Bind<&EventChoiceBase::OnChoiceResult>(*this, "choice");

	pChoiceBox->SetChoiceButton(ChoiceInfos);
	
	pHUD->AddSlate(pChoiceBox);

	//pChoiceBox->SetVisibility(true);
}
void EventChoiceBase::CloseChoiceUI()
{
	if (pChoiceBox)
	{
		//pChoiceBox->SetVisibility(true);
		auto& pHUD = GetWorld()->GetHUD();
		pHUD->RemoveSlate(pChoiceBox);
		pChoiceBox.reset();
		pChoiceBox = nullptr;
	}
}
