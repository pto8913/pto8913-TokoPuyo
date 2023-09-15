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
	LandmarkUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse,
		const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted
	);
	virtual ~LandmarkUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	void AnimationStart();
	void Completed();

	virtual std::string GetName() const override
	{
		return "LandmarkUI_" + std::to_string(mID);
	}
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