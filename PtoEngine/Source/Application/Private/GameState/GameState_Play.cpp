
#include "GameState/GameState_Play.h"

#include "UI/GameProgressUI.h"
#include "UI/GameOverUI.h"

#include "Framework/PlayerController.h"
#include "Framework/World.h"

#include "Level/Level_TokoPuyo.h"

GameState_Play::GameState_Play()
{
	mGameProgress = EGameProgress::Wait;
}
GameState_Play::~GameState_Play()
{
	OnGameProgressChanged.ClearBind();

	if (IsValid(pGameProgressUI))
	{
		pGameProgressUI->RemoveFromParent();
		pGameProgressUI->MarkPendingKill();
	}
	pGameProgressUI = nullptr;
	if (IsValid(pGameOverUI))
	{
		pGameOverUI->RemoveFromParent();
		pGameOverUI->MarkPendingKill();
	}
	pGameOverUI = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameState_Play::BeginPlay(DirectX11& dx)
{
	GameState::BeginPlay(dx);

	SetGameProgress(dx, EGameProgress::Control);
}

void GameState_Play::SetGameProgress(DirectX11& dx, EGameProgress NewState)
{
	mGameProgress = NewState;

	switch (mGameProgress)
	{
	case EGameProgress::GameOver:
		if (pGameOverUI == nullptr)
		{
			pGameOverUI = GetWorld()->CreateWidget<GameOverUI>(
				GetWorld(),
				dx,
				GetWorld()->GetPlayerController()->GetMouse(),
				mMaxScore,
				mMaxCombo
			);
			pGameOverUI->OnClickedRestart.Bind<&GameState_Play::OnClickedRestartFromGameOver>(*this, "GameState");
		}
		pGameOverUI->SetScore(mMaxScore, mMaxCombo);
		pGameOverUI->AddToViewport();
		if (pGameProgressUI)
		{
			pGameProgressUI->RemoveFromParent();
		}
		break;
	default:
		if (pGameProgressUI == nullptr)
		{
			pGameProgressUI = GetWorld()->CreateWidget<GameProgressUI>(
				GetWorld(),
				dx,
				GetWorld()->GetPlayerController()->GetMouse()
			);
			pGameProgressUI->OnClickedRestart.Bind<&GameState_Play::OnClickedRestart>(*this, "GameState");
			pGameProgressUI->OnClickedPause.Bind<&GameState_Play::OnClickedPause>(*this, "GameState");
		}
		pGameProgressUI->AddToViewport();
		if (pGameOverUI)
		{
			pGameOverUI->RemoveFromParent();
		}
		break;
	}

	OnGameProgressChanged.Broadcast(mGameProgress);
}
const EGameProgress& GameState_Play::GetGameProgress() const
{
	return mGameProgress;
}

void GameState_Play::UpdateNextPuyo(uint8_t nPuyo1_1, uint8_t nPuyo1_2, uint8_t nPuyo2_1, uint8_t nPuyo2_2)
{
	if (pGameProgressUI != nullptr)
	{
		pGameProgressUI->UpdateNextPuyo(nPuyo1_1, nPuyo1_2, nPuyo2_1, nPuyo2_2);
	}
}
void GameState_Play::UpdateScore(const int& inScore, const int& inCombo)
{
	mMaxCombo = max(inCombo, mMaxCombo);
	mMaxScore = max(inScore, mMaxScore);

	mCombo = inCombo;
	mScore = inScore;

	if (pGameProgressUI != nullptr)
	{
		pGameProgressUI->UpdateScore(mScore, mCombo, mMaxScore, mMaxCombo);
	}
}
void GameState_Play::SetAllClear(bool allClear)
{
	if (pGameProgressUI != nullptr)
	{
		pGameProgressUI->SetAllClear(allClear);
	}
}

void GameState_Play::GetResult(int& maxScore, int maxCombo)
{
	maxScore = mMaxScore;
	maxCombo = mMaxCombo;
}
void GameState_Play::GetCombo(int& combo, int maxCombo)
{
	combo = mCombo;
	maxCombo = mMaxCombo;
}
void GameState_Play::GetScore(int& score, int maxScore)
{
	score = mScore;
	maxScore = mMaxScore;
}

void GameState_Play::SetPause(bool in)
{
	bPause = in;
}
bool GameState_Play::IsPause() const noexcept
{
	return bPause;
}

void GameState_Play::OnClickedRestart(DX::MouseEvent inMouseEvent)
{
	auto level = static_cast<Level_TokoPuyo*>(GetWorld()->GetLevel());
	level->Restart();

	SetPause(false);
}
void GameState_Play::OnClickedRestartFromGameOver(DX::MouseEvent inMouseEvent)
{
	auto level = static_cast<Level_TokoPuyo*>(GetWorld()->GetLevel());
	level->Restart();

	mMaxCombo = 0;
	mMaxScore = 0;

	UpdateScore(0, 0);

	SetPause(false);
}
void GameState_Play::OnClickedPause(DX::MouseEvent inMouseEvent)
{
	auto level = static_cast<Level_TokoPuyo*>(GetWorld()->GetLevel());
	if (IsPause())
	{
		SetPause(false);
		level->Resume();
	}
	else
	{
		SetPause(true);
		level->Pause();
	}
}