#pragma once

#include "GameState.h"
#include "GameState_PlayTypes.h"

#include "Input/MouseInterface.h"

class UserWidget;

class GameProgressUI;
class GameOverUI;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameProgressChanged, const EGameProgress&);

class GameState_Play : public GameState
{
public:
	GameState_Play();
	virtual ~GameState_Play();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	virtual void SetGameProgress(DirectX11& dx, EGameProgress NewState);
	const EGameProgress& GetGameProgress() const;

	void UpdateNextPuyo(uint8_t nPuyo1_1, uint8_t nPuyo1_2, uint8_t nPuyo2_1, uint8_t nPuyo2_2);
	void UpdateScore(const int& inScore, const int& inCombo);

	void GetResult(int& maxScore, int maxCombo);
	void GetCombo(int& combo, int maxCombo);
	void GetScore(int& score, int maxScore);

	void SetPause(bool in);
	bool IsPause() const noexcept;

	std::shared_ptr<GameProgressUI> GetGameProgressUI();
	std::shared_ptr<GameOverUI> GetGameOverUI();

protected:
	void OnClickedRestart(DX::MouseEvent inMouseEvent);
	void OnClickedRestartFromGameOver(DX::MouseEvent inMouseEvent);
	void OnClickedPause(DX::MouseEvent inMouseEvent);
public:

	// ---------------------------
	// Main : Delegate
	// ---------------------------
	FOnGameProgressChanged OnGameProgressChanged;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	EGameProgress mGameProgress;

	std::shared_ptr<GameProgressUI> pGameProgressUI = nullptr;
	std::shared_ptr<GameOverUI> pGameOverUI = nullptr;

	int mMaxScore = 0;
	int mMaxCombo = 0;

	int mScore = 0;
	int mCombo = 0;

	bool bPause = false;
};