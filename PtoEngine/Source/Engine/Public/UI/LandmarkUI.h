#pragma once

#include "UI/UserWidget.h"

#include "Engine/Timer.h"
#include "Engine/Color.h"

class DirectX11;

class S_Border;
class S_TextBlock;

class LandmarkUI : public UserWidget
{
public:
	LandmarkUI(std::shared_ptr<Object> inOwner, DirectX11& dx,  
		const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted
	);
	LandmarkUI(DirectX11& dx, 
		const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted
	);
	virtual ~LandmarkUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	void AnimationStart();
	void Completed();
private:
	std::shared_ptr<S_Border> pEffectBorder = nullptr;
	std::shared_ptr<S_TextBlock> pText = nullptr;

	FTimerHandle timerHandle;

	std::wstring mLandmarkName;
	float mPlayRate = 1.f;
	FOnWidgetAnimationCompleted mDelegate;

	WidgetAnimation Anim_blackout;
	WidgetAnimationProperty<FColor> propColor;
};