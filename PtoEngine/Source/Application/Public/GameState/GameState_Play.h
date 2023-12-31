#pragma once

#include "GameState.h"
#include "GameState_PlayTypes.h"

#include "Input/MouseInterface.h"


class UserWidget;

class GameProgressUI;
class GameOverUI;


DECLARE_MULTICAST_DELEGATE(FOnRestart);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameProgressChanged, const EGameProgress&);

class GameState_Play : public GameState
{
public:
	GameState_Play();
	virtual ~GameState_Play();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameState_Play" + "#"s + std::to_string(mID);
	}
	virtual void BeginPlay(DirectX11& dx) override;

	virtual void SetGameProgress(EGameProgress NewState);
	const EGameProgress& GetGameProgress() const;

	void UpdateNextPuyo(uint8_t nPuyo1_1, uint8_t nPuyo1_2, uint8_t nPuyo2_1, uint8_t nPuyo2_2);
	void UpdateScore(const int& inScore, const int& inCombo);
	void SetAllClear(bool allClear);

	void GetResult(int& maxScore, int maxCombo);
	void GetCombo(int& combo, int maxCombo);
	void GetScore(int& score, int maxScore);

	void SetPause(bool in);
	bool IsPause() const noexcept;

protected:
	void OpenGameOverUI();
	void OpenGameProgressUI();

	void OnClickedRestart(DX::MouseEvent inMouseEvent);
	void OnClickedRestartFromGameOver(DX::MouseEvent inMouseEvent);
	void OnClickedPause(DX::MouseEvent inMouseEvent);

public:
	// ---------------------------
	// Main : Delegate
	// ---------------------------
	FOnRestart OnRestart;
	FOnGameProgressChanged OnGameProgressChanged;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	EGameProgress mGameProgress;
	EGameProgress Cached_GameProgress;

	GameProgressUI* pGameProgressUI = nullptr;
	GameOverUI* pGameOverUI = nullptr;
	DirectX11* pDX = nullptr;

	int mMaxScore = 0;
	int mMaxCombo = 0;

	int mScore = 0;
	int mCombo = 0;

	bool bPause = false;

};