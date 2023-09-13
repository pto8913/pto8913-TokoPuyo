
#include "GameState/GameState.h"
#include "Framework/World.h"

#include "Core/DirectX.h"

#include "UI/LandmarkUI.h"

#include "Engine/Timer.h"

GameState::GameState()
{
	SetTickEnabled(true);
}

GameState::~GameState()
{
	if (pLandmarkUI)
	{
		pLandmarkUI->MarkPendingKill();
	}
	pLandmarkUI = nullptr;

	OnLandmarkClosed.ClearBind();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameState::Tick(DirectX11& dx, float deltaTime)
{
	if (pLandmarkUI != nullptr)
	{
		pLandmarkUI->Tick(dx, deltaTime);
	}
}

// ---------------------------
// Main : Landmark
// ---------------------------
void GameState::OpenLandmarkUI(DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted, const float& inCompleteToDelay)
{
	FOnWidgetAnimationCompleted completed;
	if (inCompleted.IsBound())
	{
		completed.Append(inCompleted);
	}
	if (inCompleteToDelay != 0.f)
	{
		mLandmarkCompleteToDelay = inCompleteToDelay;
		completed.Bind<&GameState::LandmarkCompleted>(*this, "GameState");
	}
	else
	{
		completed.Bind<&GameState::CloseLandmarkUI>(*this, "GameState");
	}

	pLandmarkUI = CreateWidget<LandmarkUI>(GetWorld(), dx, inLandmarkName, inPlayRate, completed).get();
	pLandmarkUI->AddToViewport(2);

	GetWorld()->DeactivateHUD();
	GetWorld()->DeactivatePlayer();
	GetWorld()->DeactivateLevel();
}
void GameState::LandmarkCompleted()
{
	pLandmarkUI->RemoveFromParent();

	GetWorld()->GetTimerManager().SetTimer<&GameState::CloseLandmarkUI>(*this, 0.f, false, mLandmarkCompleteToDelay);
}
void GameState::CloseLandmarkUI()
{
	GetWorld()->ActivateLevel();
	GetWorld()->ActivateHUD();
	GetWorld()->ActivatePlayer();

	OnLandmarkClosed.Broadcast();

	pLandmarkUI->MarkPendingKill();
	pLandmarkUI = nullptr;

	OnLandmarkClosed.ClearBind();
}
