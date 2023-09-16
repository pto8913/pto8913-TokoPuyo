#pragma once

#include "UI/UserWidget.h"

class GameOverUI : public UserWidget
{
public:
	GameOverUI(
		Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse,
		const int& inMaxScore, const int& inMaxCombo
	);
	virtual ~GameOverUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void NativeOnInitialized() override;
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameOverUI" + "#"s + std::to_string(mID);
	}
	void OnClickedRestartButton(DX::MouseEvent inMouseEvent);
	void OnClickedReturnTitle(DX::MouseEvent inMouseEvent);

	// ----------------------------------------------------------
	// Main : Delegates
	// ----------------------------------------------------------
	DX::FOnMouseAction OnClickedRestart;
	DX::FOnMouseAction OnClickedPause;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	int mMaxScore;
	int mMaxCombo;
};