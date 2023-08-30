
#include "GameState/GameStateBase.h"

#include "Core/DirectX.h"

#include "UI/LandmarkUI.h"

#include "Engine/World.h"
#include "Engine/Timer.h"

GameStateBase::GameStateBase()
{
	SetTickEnabled(true);
}

GameStateBase::~GameStateBase()
{
	pLandmarkUI.reset();
	pLandmarkUI = nullptr;

	OnLandmarkClosed.ClearBind();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameStateBase::Tick(DirectX11& dx, float deltaTime)
{
	if (pLandmarkUI != nullptr)
	{
		pLandmarkUI->Tick(dx, deltaTime);
	}
}

// ---------------------------
// Main : Landmark
// ---------------------------
void GameStateBase::OpenLandmarkUI(DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted, const float& inCompleteToDelay)
{
	FOnWidgetAnimationCompleted completed;
	if (inCompleted.IsBound())
	{
		completed.Append(inCompleted);
	}
	if (inCompleteToDelay != 0.f)
	{
		mLandmarkCompleteToDelay = inCompleteToDelay;
		completed.Bind<&GameStateBase::LandmarkCompleted>(*this, "GameState");
	}
	else
	{
		completed.Bind<&GameStateBase::CloseLandmarkUI>(*this, "GameState");
	}

	pLandmarkUI = std::make_shared<LandmarkUI>(GetWorld(), dx, inLandmarkName, inPlayRate, completed);
	pLandmarkUI->AddToViewport(2);

	GetWorld()->DeactivateHUD();
	GetWorld()->DeactivatePlayer();
	GetWorld()->DeactivateLevel();
}
void GameStateBase::LandmarkCompleted()
{
	pLandmarkUI->RemoveFromParent();

	GetWorld()->GetTimerManager().SetTimer<&GameStateBase::CloseLandmarkUI>(*this, 0.f, false, mLandmarkCompleteToDelay);
}
void GameStateBase::CloseLandmarkUI()
{
	GetWorld()->ActivateLevel();
	GetWorld()->ActivateHUD();
	GetWorld()->ActivatePlayer();

	OnLandmarkClosed.Broadcast();

	pLandmarkUI.reset();
	pLandmarkUI = nullptr;

	OnLandmarkClosed.ClearBind();
}
