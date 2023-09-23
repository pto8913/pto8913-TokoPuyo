
#include "Actor/Character/Player.h"
#include "Actor/Character/Puyo.h"

#include "PtoGameInstance.h"

#include "Framework/World.h"
#include "GameState/GameState_Play.h"
#include "Level/Level_PuyoField.h"

#include "Engine/Audio.h"

Player::Player()
	: Actor(),
	InputZ(DIK_Z),
	InputX(DIK_X),
	InputLeft(DIK_LEFT),
	InputRight(DIK_RIGHT),
	InputUp(DIK_UP),
	InputDown(DIK_DOWN)
{
	const auto masterVolume = GameSettings::GetAudioVolume();
	SE_PuyoMove = std::make_shared<Audio>(L"Content/Sounds/Puyo_Move.wav");
	SE_PuyoMove->SetVolume(0.25f);
	SE_PuyoMove->SetVolume(SE_PuyoMove->GetDefaultVolume() * masterVolume / 10.f);

	SE_PuyoBottom = std::make_shared<Audio>(L"Content/Sounds/Puyo_Bottom.wav");
	SE_PuyoBottom->SetVolume(0.25f);
	SE_PuyoBottom->SetVolume(SE_PuyoBottom->GetDefaultVolume() * masterVolume / 10.f);

	SE_PuyoRotate = std::make_shared<Audio>(L"Content/Sounds/Puyo_Rotate.wav");
	SE_PuyoRotate->SetVolume(0.25f);
	SE_PuyoRotate->SetVolume(SE_PuyoRotate->GetDefaultVolume() * masterVolume / 10.f);

	PtoGameInstance& gameInstance = PtoGameInstance::Get();
	gameInstance.OnPaused.Bind<&Player::OnPaused>(*this, GetName());

	LastTime_Main = chrono::now();
	NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::Control);
}

Player::~Player()
{
	auto Stop = [](std::shared_ptr<Audio>& in)
	{
		if (in)
		{
			in->Stop();
		}
		in.reset();
		in = nullptr;
	};
	Stop(SE_PuyoBottom);
	Stop(SE_PuyoMove);
	Stop(SE_PuyoRotate);
}

void Player::NativeOnInitialized()
{
	Actor::NativeOnInitialized();

	pGameState = static_cast<GameState_Play*>(GetWorld()->GetGameState());
	pGameState->OnRestart.Bind<&Player::OnRestart>(*this, GetName());

	pLevel = static_cast<Level_PuyoField*>(GetLevel());
	pLevel->OnSpawnPuyo.Bind<&Player::OnSpawnPuyo>(*this, GetName());

	OutputDebugStringA("NativeOnInitialized\n");
}

// -------------------------------------------------------
// Main
// -------------------------------------------------------
void Player::BeginPlay(DirectX11& dx)
{
	Actor::BeginPlay(dx);

	pDX = &dx;

}
void Player::Tick(DirectX11& dx, float deltaTime)
{
	Actor::Tick(dx, deltaTime);

	if (pGameState)
	{
		const auto gameProgress = pGameState->GetGameProgress();
		if (gameProgress != EGameProgress::GameOver)
		{
			chrono::duration DurationTime_Main = chrono::now() - LastTime_Main;
			Duration_Main = std::chrono::duration_cast<std::chrono::milliseconds>(DurationTime_Main);
			if (Duration_Main.count() >= NeedDurationTime_Main)
			{
				LastTime_Main = chrono::now();
				switch (gameProgress)
				{
				case EGameProgress::Control:
					DoFrame_Control();
					break;
				default:
					break;
				}
			}
		}
	}
}
void Player::Clear()
{
	pMainPuyo = nullptr;
	pSubPuyo = nullptr;
}

// ------------------------------------------------------------
// Main : Control Puyo
// ------------------------------------------------------------
void Player::DoFrame_Control()
{
#if _DEBUG
	//OutputDebugStringA("DoFrame_Control\n");
#endif
	ActionActivePuyoSlide(0.f, 0.5f);
}

// ---------------------------
// Main : Puyo Moving
// ---------------------------
void Player::ActionActivePuyoSlide(float rateX, float rateY)
{
	if (pMainPuyo == nullptr) return;

	SE_PuyoMove->Play();

	bool success = false;
	const auto currMainIdx = pMainPuyo->Get2DIdx();
	const auto currSubIdx = pSubPuyo->Get2DIdx();
	const auto nextMainIdx = currMainIdx + FVector2D(rateX, rateY);

	float mainNextX = currMainIdx.x + rateX;
	float mainNextY = currMainIdx.y + rateY;
	if (rateY > 0.f)
	{
		mainNextY += 1.f;
	}

	float subNextX = currSubIdx.x + rateX;
	float subNextY = currSubIdx.y + rateY;

	switch (pMainPuyo->GetPuyoInfos().GetRotation())
	{
	case FPuyoInfos::ERotation::B:
	{
		if (rateY > 0.f)
		{
			subNextY += 1.f;
		}
		if (pLevel->IsValidIndex(subNextX, subNextY))
		{
			if (!pLevel->IsValidStackedPuyo(subNextX, subNextY))
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	case FPuyoInfos::ERotation::U:
	{
		if (pLevel->IsValidIndex(mainNextX, mainNextY))
		{
			if (!pLevel->IsValidStackedPuyo(mainNextX, mainNextY))
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	case FPuyoInfos::ERotation::L:
	{
		if (pLevel->IsValidIndex(mainNextX, mainNextY) && pLevel->IsValidIndex(subNextX, subNextY))
		{
			if (!pLevel->IsValidStackedPuyo(mainNextX, mainNextY) && !pLevel->IsValidStackedPuyo(subNextX, subNextY))
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	case FPuyoInfos::ERotation::R:
	{
		if (pLevel->IsValidIndex(mainNextX, mainNextY) && pLevel->IsValidIndex(subNextX, subNextY))
		{
			if (!pLevel->IsValidStackedPuyo(mainNextX, mainNextY) && !pLevel->IsValidStackedPuyo(subNextX, subNextY))
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	default:
		break;
	}

	if (!success)
	{
		if (rateY > 0.f)
		{
			ActivePuyoDownToRelease();
		}
	}
}
void Player::ActivePuyoDownToRelease()
{
#if _DEBUG
	OutputDebugStringA("ActivePuyoDownToRelease\n");
#endif
	auto currMainIdx = pMainPuyo->Get2DIdx();
	currMainIdx.y = floor(currMainIdx.y);
	if (currMainIdx.x < 0)
	{
		currMainIdx.x = 0;
	}
	else if (currMainIdx.x >= pLevel->GetWidth())
	{
		currMainIdx.x = floor(currMainIdx.x);
	}
	if (pLevel->IsValidIndex(currMainIdx.x, currMainIdx.y))
	{
		if (!pLevel->IsValidStackedPuyo(currMainIdx.x, currMainIdx.y))
		{
			UpdateActivePuyo(currMainIdx.x, currMainIdx.y);
		}
	}

	SE_PuyoBottom->Play();
	pGameState->SetGameProgress(EGameProgress::Release);
}

void Player::ActionActivePuyoRotate(bool rotateR)
{
	SE_PuyoRotate->Play();

	auto mainIdx = pMainPuyo->Get2DIdx();
	auto c_mainIdx = mainIdx;
	/* set rotate coord. */
	if (rotateR)
	{
		/* Rotate Right */
		switch (pMainPuyo->GetPuyoInfos().GetRotation())
		{
		case FPuyoInfos::ERotation::U:
			if (!pLevel->IsValidIndex(mainIdx.x + 1, mainIdx.y) || (pLevel->IsValidIndex(mainIdx.x + 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x + 1, mainIdx.y)))
			{
				/* U to B */
				if (pLevel->IsValidStackedPuyo(mainIdx.x - 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x + 1, mainIdx.y))
				{
					if (pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 1))
					{
#if _DEBUG
						OutputDebugStringA("U to B by R\n");
#endif
						if (pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y + 1))
						{
							mainIdx.y -= 1;
						}
						pMainPuyo->GetPuyoInfos().Rotate(rotateR);
					}
				}
				else
				{
					/* U to R */
#if _DEBUG
					OutputDebugStringA("U to R by R\n");
#endif
					mainIdx.x -= 1;
				}
			}
			break;
		case FPuyoInfos::ERotation::B:
			if (!pLevel->IsValidIndex(mainIdx.x - 1, mainIdx.y) || (pLevel->IsValidIndex(mainIdx.x - 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x - 1, mainIdx.y)))
			{
				/* B to U */
				if (pLevel->IsValidStackedPuyo(mainIdx.x - 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x + 1, mainIdx.y))
				{
					if (pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 1))
					{
#if _DEBUG
						OutputDebugStringA("B to U by R\n");
#endif
						if (!pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y + 1))
						{
						}
						//mainIdx.y -= 1;
						pMainPuyo->GetPuyoInfos().Rotate(rotateR);
					}
				}
				else
				{
					/* B to R */
#if _DEBUG
					OutputDebugStringA("B to R by R\n");
#endif
					mainIdx.x += 1;
				}
			}
			break;
		case FPuyoInfos::ERotation::L:
			break;
		case FPuyoInfos::ERotation::R:
			if (!pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 2) || (pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 2) && pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y + 2)))
			{
				mainIdx.y -= 1;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		/* Rotate Left */
		switch (pMainPuyo->GetPuyoInfos().GetRotation())
		{
		case FPuyoInfos::ERotation::U:
			if (!pLevel->IsValidIndex(mainIdx.x - 1, mainIdx.y) || (pLevel->IsValidIndex(mainIdx.x - 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x - 1, mainIdx.y)))
			{
				if (pLevel->IsValidStackedPuyo(mainIdx.x - 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x + 1, mainIdx.y))
				{
					/* U to B */
					if (pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 1))
					{
#if _DEBUG
						OutputDebugStringA("U to B by L\n");
#endif
						if (pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y + 1))
						{
							mainIdx.y -= 1;
						}
						pMainPuyo->GetPuyoInfos().Rotate(rotateR);
					}
				}
				else
				{
#if _DEBUG
					OutputDebugStringA("U to L by L\n");
#endif
					mainIdx.x += 1;
				}
			}
			break;
		case FPuyoInfos::ERotation::B:
			if (!pLevel->IsValidIndex(mainIdx.x + 1, mainIdx.y) || (pLevel->IsValidIndex(mainIdx.x + 1, mainIdx.y) && pLevel->IsValidStackedPuyo(mainIdx.x + 1, mainIdx.y)))
			{
				/* B to U */
				if (pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 1))
				{
#if _DEBUG
					OutputDebugStringA("B to U by L\n");
#endif
					if (pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y + 1))
					{
						mainIdx.y -= 1;
					}
					pMainPuyo->GetPuyoInfos().Rotate(rotateR);
				}
				else
				{
#if _DEBUG
					OutputDebugStringA("B to L by L\n");
#endif
					mainIdx.x -= 1;
				}
			}
			break;
		case FPuyoInfos::ERotation::L:
			if (!pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 2) || (pLevel->IsValidIndex(mainIdx.x, mainIdx.y + 2) && pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y + 2)))
			{
				mainIdx.y -= 1;
			}
			break;
		case FPuyoInfos::ERotation::R:
			break;
		default:
			break;
		}
	}

	if (pLevel->IsValidIndex(mainIdx.x, mainIdx.y) && !pLevel->IsValidStackedPuyo(mainIdx.x, mainIdx.y))
	{
		pMainPuyo->GetPuyoInfos().Rotate(rotateR);
		UpdateActivePuyo(mainIdx.x, mainIdx.y);
	}
}
void Player::ActivePuyoRotateToRelease()
{
	auto idx = pMainPuyo->Get2DIdx();
	if (idx.y >= pLevel->GetHeight())
	{
		idx.y = floor(idx.y);
	}
	else
	{
		idx.y = ceil(idx.y);
	}
	if (idx.x < 0)
	{
		idx.x = 0;
	}
	else if (idx.x >= pLevel->GetWidth())
	{
		idx.x = floor(idx.x);
	}

	UpdateActivePuyo(idx.x, idx.y);
	pGameState->SetGameProgress(EGameProgress::Release);
}

void Player::UpdateActivePuyo(const float& x, const float& y)
{
	pLevel->SetSpriteLocation(pMainPuyo, x, y);
	UpdateSubPuyoLocationByRotation();
}
void Player::UpdateSubPuyoLocationByRotation()
{
	const auto mainIdx = pMainPuyo->Get2DIdx();
	switch (pMainPuyo->GetPuyoInfos().GetRotation())
	{
	case FPuyoInfos::ERotation::U:
		pLevel->SetSpriteLocation(pSubPuyo, mainIdx.x, mainIdx.y - 1);
		break;
	case FPuyoInfos::ERotation::B:
		pLevel->SetSpriteLocation(pSubPuyo, mainIdx.x, mainIdx.y + 1);
		break;
	case FPuyoInfos::ERotation::L:
		pLevel->SetSpriteLocation(pSubPuyo, mainIdx.x - 1, mainIdx.y);
		break;
	case FPuyoInfos::ERotation::R:
		pLevel->SetSpriteLocation(pSubPuyo, mainIdx.x + 1, mainIdx.y);
		break;
	default:
		break;
	}
}

// ----------------------
// Main : Input
// ----------------------
void Player::InputUpdate()
{
	if (pGameState->GetGameProgress() == EGameProgress::Control)
	{
		if (InputZ)
		{
			ActionActivePuyoRotate(false);
		}
		if (InputX)
		{
			ActionActivePuyoRotate();
		}
		if (InputLeft)
		{
			ActionActivePuyoSlide(-1.f, 0.f);
		}
		if (InputRight)
		{
			ActionActivePuyoSlide(1.f, 0.f);
		}
		if (InputUp)
		{
			ActionActivePuyoRotate();
		}
		if (InputDown)
		{
			pLevel->SetNeedDurationCached(GameSettings::GetPuyoFallSpeed(EPuyoControl::PressDown));
		}
		else
		{
			pLevel->ResetNeedDuration();
		}
	}
}

void Player::OnPaused(bool inPause)
{
	if (inPause)
	{

	}
	else
	{

	}
}

void Player::OnSpawnPuyo(const uint8_t& mainId, const uint8_t& subId)
{
	OutputDebugStringA("OnSpawnPuyo\n");

	if (pMainPuyo == nullptr)
	{
#if _DEBUG
		OutputDebugStringA("SpawnPuyo new pointer\n");
#endif
		auto world = GetWorld();
		pMainPuyo = world->SpawnActor<Puyo>(
			*pDX, mainId
		);
		pSubPuyo = world->SpawnActor<Puyo>(
			*pDX, subId
		);
	}
	else
	{
#if _DEBUG
		OutputDebugStringA("SpawnPuyo shared pointer\n");
#endif
		pMainPuyo->SetType(mainId);
		pSubPuyo->SetType(subId);
	}
	UpdateActivePuyo(2, 1);
}
void Player::OnRestart()
{
	Clear();
}
void Player::OnGameProgressChanged(const EGameProgress& NewState)
{
	switch (NewState)
	{
	case EGameProgress::Release:
		pLevel->SetActivePuyo(std::move(pMainPuyo), std::move(pSubPuyo));
		pMainPuyo = nullptr;
		pSubPuyo = nullptr;
		break;
	default:
		break;
	}
}
