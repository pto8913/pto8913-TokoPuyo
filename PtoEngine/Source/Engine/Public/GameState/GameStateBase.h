#pragma once

#include "Object/Actor.h"

#include "UI/WidgetAnimationTypes.h"

class DirectX11;
class LandmarkUI;

DECLARE_MULTICAST_DELEGATE(FOnLandmarkStateChanged);

/*
* 
* 
* 
* 
*/

class GameStateBase : public Actor
{
public:
	GameStateBase();
	virtual ~GameStateBase();

	GameStateBase(const GameStateBase&) = delete;
	GameStateBase& operator=(const GameStateBase&) = delete;
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	// ---------------------------
	// Main : Landmark
	// ---------------------------
	virtual void OpenLandmarkUI(
		DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, 
		const FOnWidgetAnimationCompleted& inCompleted = {},
		const float& inCompleteToDelay = 0.01f
	);
	virtual void LandmarkCompleted();
	void CloseLandmarkUI();

	// ---------------------------
	// Main : Delegate
	// ---------------------------
	FOnLandmarkStateChanged OnLandmarkClosed;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------

	// ---------------------------
	// State : Landmark
	// ---------------------------
	std::shared_ptr<LandmarkUI> pLandmarkUI = nullptr;
	float mLandmarkCompleteToDelay = 0.01f;
};