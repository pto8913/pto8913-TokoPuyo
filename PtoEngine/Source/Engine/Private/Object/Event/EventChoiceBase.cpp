
#include "Object/Event/EventChoiceBase.h"

#include "UI/Message/S_ChoiceBox.h"

#include "GameSettings.h"

#include "Engine/World.h"
#include "Controller/PlayerController.h"
#include "UI/HUD.h"

using namespace DirectX;

EventChoiceBase::EventChoiceBase(DirectX11& dx, const EEventId& inEventType)
	: EventBase(dx, inEventType)
{
}
EventChoiceBase::~EventChoiceBase()
{
	CloseChoiceUI();
}

void EventChoiceBase::EnterVolume(const int& x, const int& y)
{
	OpenChoiceUI();
}
void EventChoiceBase::LeaveVolume(const int& x, const int& y)
{
	CloseChoiceUI();
}

void EventChoiceBase::OpenChoiceUI()
{
	if (pChoiceBox == nullptr)
	{
		auto& pHUD = GetWorld()->GetPlayerController()->GetHUD();
		pChoiceBox = std::make_shared<S_ChoiceBox>(pHUD->GetRt2D());
		pChoiceBox->GetOnChoiceResult().Bind<&EventChoiceBase::OnChoiceResult>(*this, "choice");
		InitChoiceBox();
		pHUD->AddSlate(pChoiceBox);
	}
	pChoiceBox->SetChoiceButton(ChoiceInfos);
	//pChoiceBox->SetVisibility(true);
}
void EventChoiceBase::CloseChoiceUI()
{
	if (pChoiceBox)
	{
		//pChoiceBox->SetVisibility(true);
		auto& pHUD = GetWorld()->GetPlayerController()->GetHUD();
		pHUD->RemoveSlate(pChoiceBox);
		pChoiceBox.reset();
		pChoiceBox = nullptr;
	}
}
void EventChoiceBase::InitChoiceBox()
{
	if (pChoiceBox)
	{
		pChoiceBox->SetSize(XMFLOAT2(200.f, ChoiceInfos.Size() * 30.f));
		pChoiceBox->SetPosition(XMFLOAT2(
			GameSettings::GAMEUI_LEFT_TOP.x - pChoiceBox->GetSize().x - GameSettings::GAMESCREEN_LEFT_TOP.x,
			GameSettings::GAMESCREEN_LEFT_TOP.y)
		);
	}
}