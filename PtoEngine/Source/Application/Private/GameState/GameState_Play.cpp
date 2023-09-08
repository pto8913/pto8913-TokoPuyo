
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

	pGameProgressUI.reset();
	pGameProgressUI = nullptr;

	pGameOverUI.reset();
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
void GameState_Play::Tick(DirectX11& dx, float deltaSec)
{
	GameState::Tick(dx, deltaSec);
	
	switch (mGameProgress)
	{
	case EGameProgress::GameOver:
		if (pGameOverUI != nullptr)
		{
			pGameOverUI->Tick(dx, deltaSec);
		}
		break;
	default:
		if (pGameProgressUI != nullptr)
		{
			pGameProgressUI->Tick(dx, deltaSec);
		}
		break;
	}
}

void GameState_Play::SetGameProgress(DirectX11& dx, EGameProgress NewState)
{
	mGameProgress = NewState;

	switch (mGameProgress)
	{
	case EGameProgress::GameOver:
		if (pGameOverUI == nullptr)
		{
			pGameOverUI = std::make_shared<GameOverUI>(
				GetWorld(),
				dx,
				GetWorld()->GetPlayerController()->GetMouse(),
				mMaxScore,
				mMaxCombo
			);
			pGameOverUI->AddToViewport();
		}
		break;
	default:
		if (pGameProgressUI == nullptr)
		{
			pGameProgressUI = std::make_shared<GameProgressUI>(
				GetWorld(),
				dx,
				GetWorld()->GetPlayerController()->GetMouse()
			);

			pGameProgressUI->OnClickedRestart.Bind<&GameState_Play::OnClickedRestart>(*this, "GameState");
			pGameProgressUI->AddToViewport();
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
		pGameProgressUI->UpdateScore(inScore, inCombo);
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

void GameState_Play::SetIsPause(bool in)
{
	pGameProgressUI->SetIsPause(in);
}
bool GameState_Play::IsPause() const noexcept
{
	return pGameProgressUI->IsPause();
}

std::shared_ptr<GameProgressUI> GameState_Play::GetGameProgressUI()
{
	return pGameProgressUI;
}
std::shared_ptr<GameOverUI> GameState_Play::GetGameOverUI()
{
	return pGameOverUI;
}

void GameState_Play::OnClickedRestart(DX::MouseEvent inMouseEvent)
{
	auto level = static_pointer_cast<Level_TokoPuyo>(GetWorld()->GetLevel());
	level->Restart();

	mScore = 0;
	mCombo = 0;

	SetIsPause(false);
}
void GameState_Play::OnClickedPause(DX::MouseEvent inMouseEvent)
{
	SetIsPause(true);
}