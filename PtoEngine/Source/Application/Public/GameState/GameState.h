#pragma once

#include "Framework/GameStateBase.h"

#include "UI/WidgetAnimationTypes.h"

class LandmarkUI;

DECLARE_MULTICAST_DELEGATE(FOnLandmarkStateChanged);

/*
*
*
*
*
*/
class GameState : public GameStateBase
{
public:
	GameState();
	virtual ~GameState();

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		return "GameState_" + std::to_string(mID);
	}
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
	LandmarkUI* pLandmarkUI = nullptr;
	float mLandmarkCompleteToDelay = 0.01f;
};