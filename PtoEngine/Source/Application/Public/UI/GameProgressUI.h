#pragma once

#include "UI/UserWidget.h"
#include "Engine/Color.h"

class S_TextBlock;
class S_Image;
class S_Button;
class S_Overlay;

class GameProgressUI : public UserWidget
{
public:
	GameProgressUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~GameProgressUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void NativeOnInitialized() override;
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameProgressUI" + "#"s + std::to_string(mID);
	}
	void UpdateNextPuyo(uint8_t nPuyo1_1, uint8_t nPuyo1_2, uint8_t nPuyo2_1, uint8_t nPuyo2_2);
	void UpdateScore(const int& inScore, const int& inCombo, const int& inMaxScore, const int& inMaxCombo);
	void SetAllClear(bool allClear);
protected:
	void OnClickedRestartButton(DX::MouseEvent inMouseEvent);
	void OnClickedPauseButton(DX::MouseEvent inMouseEvent);
	void OnClickedReturnToTitle(DX::MouseEvent inMouseEvent);

	void ComboAnimationCompleted();
	void AllClearAnimationCompleted();
public:

	// ----------------------------------------------------------
	// Main : Delegates
	// ----------------------------------------------------------
	DX::FOnMouseAction OnClickedRestart;
	DX::FOnMouseAction OnClickedPause;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	
	// ----------------------------------------------------------
	// State : Game State
	// ----------------------------------------------------------
	std::shared_ptr<S_Overlay> pOverlay_NextPuyo = nullptr;
	std::shared_ptr<S_Image> pImage_NextPuyo1_1 = nullptr;
	std::shared_ptr<S_Image> pImage_NextPuyo1_2 = nullptr;

	std::shared_ptr<S_Image> pImage_NextPuyo2_1 = nullptr;
	std::shared_ptr<S_Image> pImage_NextPuyo2_2 = nullptr;

	std::shared_ptr<S_TextBlock> pTextBlock_MaxScore = nullptr;
	std::shared_ptr<S_TextBlock> pTextBlock_MaxCombo = nullptr;

	std::shared_ptr<S_TextBlock> pTextBlock_Score = nullptr;
	std::shared_ptr<S_TextBlock> pTextBlock_Combo = nullptr;

	std::shared_ptr<S_Button> pButton_Pause = nullptr;

	// ----------------------------------------------------------
	// State : Effect
	// ----------------------------------------------------------
	std::shared_ptr<S_TextBlock> pTextBlock_AllClearEffect = nullptr;
	std::shared_ptr<S_TextBlock> pTextBlock_ComboEffect = nullptr;
	WidgetAnimationProperty<FColor> propColor;

	std::shared_ptr<WidgetAnimation> mAnimCombo = nullptr;
	std::shared_ptr<WidgetAnimation> mAnimAllClear = nullptr;

	bool bPause = false;
};