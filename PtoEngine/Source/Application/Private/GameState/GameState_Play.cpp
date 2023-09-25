
#include "GameState/GameState_Play.h"

#include "UI/GameProgressUI.h"
#include "UI/GameOverUI.h"

#include "Framework/PlayerController.h"
#include "Framework/World.h"

#include "PtoGameInstance.h"

GameState_Play::GameState_Play()
{
	mGameProgress = EGameProgress::Wait;
	Cached_GameProgress = EGameProgress::Invalid;
}
GameState_Play::~GameState_Play()
{
	OnGameProgressChanged.ClearBind();
	OnRestart.ClearBind();

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
	pDX = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameState_Play::BeginPlay(DirectX11& dx)
{
	pDX = &dx;

	GameState::BeginPlay(dx);

	SetGameProgress(EGameProgress::Control);

	OnRestart.Broadcast();
}

void GameState_Play::SetGameProgress(EGameProgress NewState)
{
	mGameProgress = NewState;
	if (NewState != EGameProgress::GameOver)
	{
		PtoGameInstance& gameInstance = PtoGameInstance::Get();
		gameInstance.SetBGM(true);
	}
	switch (mGameProgress)
	{
	case EGameProgress::GameOver:
		OpenGameOverUI();
		break;
	default:
		OpenGameProgressUI();
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
	if (bPause)
	{
		PtoGameInstance& gameInstance = PtoGameInstance::Get();
		gameInstance.SetPause(true);

		Cached_GameProgress = GetGameProgress();
		SetGameProgress(EGameProgress::Wait);
	}
	else
	{
		PtoGameInstance& gameInstance = PtoGameInstance::Get();
		gameInstance.SetPause(false);

		if (Cached_GameProgress != EGameProgress::Invalid)
		{
			SetGameProgress(Cached_GameProgress);
		}
	}
}
bool GameState_Play::IsPause() const noexcept
{
	return bPause;
}

void GameState_Play::OpenGameOverUI()
{
	if (pGameOverUI == nullptr)
	{
		pGameOverUI = GetWorld()->CreateWidget<GameOverUI>(
			GetWorld(),
			*pDX,
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
		//pGameProgressUI->RemoveFromParent();
	}
}
void GameState_Play::OpenGameProgressUI()
{
	if (pGameProgressUI == nullptr)
	{
		pGameProgressUI = GetWorld()->CreateWidget<GameProgressUI>(
			GetWorld(),
			*pDX,
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
}

void GameState_Play::OnClickedRestart(DX::MouseEvent inMouseEvent)
{
	OnRestart.Broadcast();

	SetPause(false);
}
void GameState_Play::OnClickedRestartFromGameOver(DX::MouseEvent inMouseEvent)
{
	OnRestart.Broadcast();

	mMaxCombo = 0;
	mMaxScore = 0;

	UpdateScore(0, 0);

	SetPause(false);
}
void GameState_Play::OnClickedPause(DX::MouseEvent inMouseEvent)
{
	if (IsPause())
	{
		SetPause(false);
	}
	else
	{
		SetPause(true);
	}
}

