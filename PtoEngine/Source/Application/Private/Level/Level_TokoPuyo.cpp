
#include "Level/Level_TokoPuyo.h"

#include "Framework/World.h"
#include "Core/DirectX.h"

#include "Actor/Character/Puyo.h"
#include "Actor/Character/PuyoTypes.h"
#include "Actor/Ground/GroundBase.h"

#include "Math/Math.h"

#include "Engine/Audio.h"

#include "GameState/GameState_Play.h"

using namespace DirectX;

Level_TokoPuyo::Level_TokoPuyo(DirectX11& dx)
	: Level2D(dx),
	InputZ(DIK_Z),
	InputX(DIK_X),
	InputLeft(DIK_LEFT),
	InputRight(DIK_RIGHT),
	InputUp(DIK_UP),
	InputDown(DIK_DOWN)
{
	pDX = &dx;

	mGameBoardSize = GameSettings::GetGameBoardSize();
	Init(mGameBoardSize.x, mGameBoardSize.y);

	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;

	Cached_NeedDurationTime_Main = 0;

	BGM = std::make_shared<Audio>(L"Content/Sounds/Puyo_BGM.wav");
	SE_PuyoMove = std::make_shared<Audio>(L"Content/Sounds/Puyo_Move.wav");
	SE_PuyoMove->SetVolume(0.5f);
	SE_PuyoBottom = std::make_shared<Audio>(L"Content/Sounds/Puyo_Bottom.wav");
	SE_PuyoRotate = std::make_shared<Audio>(L"Content/Sounds/Puyo_Rotate.wav");
	SE_PuyoRotate->SetVolume(0.5f);
	SE_PuyoVanish = std::make_shared<Audio>(L"Content/Sounds/Puyo_Vanish.wav");
	SE_PuyoGameOver = std::make_shared<Audio>(L"Content/Sounds/Puyo_GameOver.wav");
	if (BGM)
	{
		BGM->SetVolume(0.5f);
		BGM->SetLoop(true);
		BGM->Play();
	}

	//mGameMode = std::make_shared<GameMode>();
	//vanishCount = static_cast<uint8_t>(mGameMode->GetNumOfConnect());

	LastTime_Main = chrono::now();
}
Level_TokoPuyo::~Level_TokoPuyo()
{
	pMainPuyo.reset();
	pMainPuyo = nullptr;

	pSubPuyo.reset();
	pSubPuyo = nullptr;

	pGameState.reset();
	pGameState = nullptr;

	unionFind.clear();

	stackedPuyo.erase(stackedPuyo.begin(), stackedPuyo.end());
	stackedPuyo.clear();

	planVanishPuyo.erase(planVanishPuyo.begin(), planVanishPuyo.end());
	planVanishPuyo.clear();
	
	BGM.reset();
	BGM = nullptr;

	SE_PuyoMove.reset();
	SE_PuyoMove = nullptr;

	SE_PuyoBottom.reset();
	SE_PuyoBottom = nullptr;

	SE_PuyoRotate.reset();
	SE_PuyoRotate = nullptr;

	SE_PuyoVanish.reset();
	SE_PuyoVanish = nullptr;

	SE_PuyoGameOver.reset();
	SE_PuyoGameOver = nullptr;

	pDX = nullptr;
}

void Level_TokoPuyo::Init(const int& x, const int& y)
{
	width = x;
	height = y;
	const auto size = (int)(x * y);
	unionFind = UnionFind(size);
	stackedPuyo.resize(size);
	planVanishPuyo.resize(size);
}

void Level_TokoPuyo::GenerateGroundLayer()
{
	auto world = GetWorld();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			auto ground = world->SpawnActor<GroundBase>(*pDX, EGroundId::Background);
			SetSpriteLocation(ground, x, y);
		}
	}
}

void Level_TokoPuyo::BeginPlay(DirectX11& dx)
{
	Level2D::BeginPlay(dx);

	pGameState = static_pointer_cast<GameState_Play>(GetWorld()->GetGameState());
	pGameState->OnGameProgressChanged.Bind<&Level_TokoPuyo::GameProgressChanged>(*this, "Level_TokoPuyo");
	GameProgressChanged(pGameState->GetGameProgress());

	Restart();
}
void Level_TokoPuyo::Tick(DirectX11& dx, float deltaTime)
{
	const auto gameProgress = pGameState->GetGameProgress();
	if (gameProgress != EGameProgress::GameOver)
	{
		Level2D::Tick(dx, deltaTime);

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
			case EGameProgress::Release:
				DoFrame_Release();
				break;
			case EGameProgress::Vanish:
				DoFrame_Vanish();
				break;
			case EGameProgress::FallAll:
				DoFrame_FallAll();
				break;
			case EGameProgress::GameOver:
				break;
			case EGameProgress::GameOverAnimation:
				break;
			default:
				break;
			}
		}

		InputUpdate();
	}
}

void Level_TokoPuyo::Restart()
{
	pGameState->SetGameProgress(*pDX, EGameProgress::Wait);

	if (pMainPuyo != nullptr)
	{
		pMainPuyo->DestroyActor();
	}
	pMainPuyo.reset();
	pMainPuyo = nullptr;

	if (pSubPuyo)
	{
		pSubPuyo->DestroyActor();
	}
	pSubPuyo.reset();
	pSubPuyo = nullptr;

	nextPuyo1_1 = GameSettings::EMPTY_PUYO;
	nextPuyo1_2 = GameSettings::EMPTY_PUYO;
	nextPuyo2_1 = GameSettings::EMPTY_PUYO;
	nextPuyo2_2 = GameSettings::EMPTY_PUYO;

	unionFind.clear();

	for (int y = 0; y < mGameBoardSize.y; ++y)
	{
		for (int x = 0; x < mGameBoardSize.x; ++x)
		{
			if (IsValidIndex(stackedPuyo, x, y))
			{
				if (auto puyoActor = stackedPuyo[y][x])
				{
					puyoActor->DestroyActor();
				}
			}
		}
	}
	stackedPuyo.clear();

	planVanishPuyo.erase(planVanishPuyo.begin(), planVanishPuyo.end());
	planVanishPuyo.clear();

	std::vector<std::shared_ptr<Puyo>> puyoRow(width, nullptr);
	stackedPuyo.assign(height, puyoRow);

	ResetPlanToVanishPuyo();

	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;

	LastTime_Main = chrono::now();

	StartControlPuyo();
}
void Level_TokoPuyo::Pause()
{
	if (pGameState->IsPause())
	{
		Cached_GameProgress = pGameState->GetGameProgress();
		pGameState->SetGameProgress(*pDX, EGameProgress::Wait);
	}
}
void Level_TokoPuyo::Resume()
{
	if (!pGameState->IsPause())
	{
		pGameState->SetGameProgress(*pDX, Cached_GameProgress);
	}
}

void Level_TokoPuyo::GameProgressChanged(const EGameProgress& NewState)
{
	if (NewState != EGameProgress::GameOver)
	{
		if (!BGM->IsPlaying())
		{
			BGM->Play();
		}
	}
	switch (NewState)
	{
	case EGameProgress::Control:
		NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::Control);
		break;
	case EGameProgress::Release:
		NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::Release);
		DoFrame_Release();
		break;
	case EGameProgress::Vanish:
		StartTime_Vanish = chrono::now();
		NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::Vanish);
		break;
	case EGameProgress::FallAll:
		NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::VanishToFall);
		break;
	case EGameProgress::GameOver:
		BGM->Stop();
		SE_PuyoGameOver->Play();
		break;
	default:
		NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::Wait);
		break;
	}
}

void Level_TokoPuyo::StartControlPuyo()
{
	OutputDebugStringA("Restart Puyo\n");

	const auto idx = GameSettings::GetGameOverCoord();
	if (IsValidIndex(stackedPuyo, idx.x, idx.y))
	{
		if (GetStackedPuyo(idx.x, idx.y) != nullptr)
		{
			pGameState->SetGameProgress(*pDX, EGameProgress::GameOver);
		}
		else
		{
			pGameState->SetGameProgress(*pDX, EGameProgress::Control);

			ResetCalcScoreCount();
			SpawnPuyo();
		}
	}
	else
	{
		pGameState->SetGameProgress(*pDX, EGameProgress::GameOver);
	}
}
void Level_TokoPuyo::SpawnPuyo()
{
	auto mainPuyoId = GameSettings::EMPTY_PUYO;
	auto subPuyoId = GameSettings::EMPTY_PUYO;
	if (nextPuyo1_1 == GameSettings::EMPTY_PUYO)
	{
		mainPuyoId = Random();
		subPuyoId = Random();

		nextPuyo1_1 = Random();
		nextPuyo1_2 = Random();
	}
	else
	{
		mainPuyoId = std::move(nextPuyo1_1);
		subPuyoId = std::move(nextPuyo1_2);

		nextPuyo1_1 = std::move(nextPuyo2_1);
		nextPuyo1_2 = std::move(nextPuyo2_2);
	}
	nextPuyo2_1 = Random();
	nextPuyo2_2 = Random();

	if (pMainPuyo == nullptr)
	{
#if _DEBUG
		OutputDebugStringA("SpawnPuyo new pointer");
#endif
		auto world = GetWorld();
		pMainPuyo = world->SpawnActor<Puyo>(
			*pDX, mainPuyoId
		);
		pSubPuyo = world->SpawnActor<Puyo>(
			*pDX, subPuyoId
		);
	}
	else
	{
#if _DEBUG
		OutputDebugStringA("SpawnPuyo shared pointer");
#endif
		pMainPuyo->SetType(mainPuyoId);
		pSubPuyo->SetType(subPuyoId);
	}
	pGameState->UpdateNextPuyo(nextPuyo1_1, nextPuyo1_2, nextPuyo2_1, nextPuyo2_2);

	UpdateActivePuyo(2, 0);
}

// ------------------------------------------------------------
// Main : Control Puyo
// ------------------------------------------------------------
void Level_TokoPuyo::DoFrame_Control()
{
#if _DEBUG
	//OutputDebugStringA("DoFrame_Control\n");
#endif
	ActionActivePuyoSlide(0.f, 0.5f);
}

// ---------------------------
// Main : Puyo Moving
// ---------------------------
void Level_TokoPuyo::ActionActivePuyoSlide(float rateX, float rateY)
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

	switch (pMainPuyo->GetRotation())
	{
	case Puyo::ERotation::B:
	{
		if (rateY > 0.f)
		{
			subNextY += 1.f;
		}
		if (IsValidIndex(stackedPuyo, subNextX, subNextY))
		{
			if (GetStackedPuyo(subNextX, subNextY) == nullptr)
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	case Puyo::ERotation::U:
	{
		if (IsValidIndex(stackedPuyo, mainNextX, mainNextY))
		{
			if (GetStackedPuyo(mainNextX, mainNextY) == nullptr)
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	case Puyo::ERotation::L:
	{
		if (IsValidIndex(stackedPuyo, mainNextX, mainNextY) && IsValidIndex(stackedPuyo, subNextX, subNextY))
		{
			if (GetStackedPuyo(mainNextX, mainNextY) == nullptr && GetStackedPuyo(subNextX, subNextY) == nullptr)
			{
				UpdateActivePuyo(nextMainIdx.x, nextMainIdx.y);
				success = true;
			}
		}
		break;
	}
	case Puyo::ERotation::R:
	{
		if (IsValidIndex(stackedPuyo, mainNextX, mainNextY) && IsValidIndex(stackedPuyo, subNextX, subNextY))
		{
			if (GetStackedPuyo(mainNextX, mainNextY) == nullptr && GetStackedPuyo(subNextX, subNextY) == nullptr)
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
void Level_TokoPuyo::ActivePuyoDownToRelease()
{
#if _DEBUG
	OutputDebugStringA("ActivePuyoDownToRelease");
#endif
	auto currMainIdx = pMainPuyo->Get2DIdx();
	currMainIdx.y = floor(currMainIdx.y);
	if (currMainIdx.x < 0)
	{
		currMainIdx.x = 0;
	}
	else if (currMainIdx.x >= mGameBoardSize.x)
	{
		currMainIdx.x = floor(currMainIdx.x);
	}
	if (IsValidIndex(stackedPuyo, currMainIdx.x, currMainIdx.y))
	{
		if (GetStackedPuyo(currMainIdx.x, currMainIdx.y) == nullptr)
		{
			UpdateActivePuyo(currMainIdx.x, currMainIdx.y);
		}
	}

	pGameState->SetGameProgress(*pDX, EGameProgress::Release);
	SE_PuyoBottom->Play();
}

void Level_TokoPuyo::ActionActivePuyoRotate(bool rotateR)
{
	SE_PuyoRotate->Play();

	auto mainIdx = pMainPuyo->Get2DIdx();
	auto c_mainIdx = mainIdx;
	/* set rotate coord. */
	if (rotateR)
	{
		switch (pMainPuyo->GetRotation())
		{
		case Puyo::ERotation::U:
#if _DEBUG
			OutputDebugStringA("U to R\n");
#endif
			if (!IsValidIndex(stackedPuyo, mainIdx.x + 1, mainIdx.y) || (IsValidIndex(stackedPuyo, mainIdx.x + 1, mainIdx.y) && GetStackedPuyo(mainIdx.x + 1, mainIdx.y) != nullptr))
			{
				mainIdx.x -= 1;
			}
			break;
		case Puyo::ERotation::B:
#if _DEBUG
			OutputDebugStringA("B to L\n");
#endif
			if (!IsValidIndex(stackedPuyo, mainIdx.x - 1, mainIdx.y) || (IsValidIndex(stackedPuyo, mainIdx.x - 1, mainIdx.y) && GetStackedPuyo(mainIdx.x - 1, mainIdx.y) != nullptr))
			{
				mainIdx.x += 1;
			}
			break;
		case Puyo::ERotation::L:
#if _DEBUG
			OutputDebugStringA("L to U\n");
#endif
			break;
		case Puyo::ERotation::R:
#if _DEBUG
			OutputDebugStringA("R to B\n");
#endif
			if (!IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y + 2) || (IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y + 2) && GetStackedPuyo(mainIdx.x, mainIdx.y + 2) != nullptr))
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
		switch (pMainPuyo->GetRotation())
		{
		case Puyo::ERotation::U:
#if _DEBUG
			OutputDebugStringA("U to L\n");
#endif
			if (!IsValidIndex(stackedPuyo, mainIdx.x - 1, mainIdx.y) || (IsValidIndex(stackedPuyo, mainIdx.x - 1, mainIdx.y) && GetStackedPuyo(mainIdx.x - 1, mainIdx.y) != nullptr))
			{
				mainIdx.x += 1;
			}
			break;
		case Puyo::ERotation::B:
#if _DEBUG
			OutputDebugStringA("B to R\n");
#endif
			if (!IsValidIndex(stackedPuyo, mainIdx.x + 1, mainIdx.y) || (IsValidIndex(stackedPuyo, mainIdx.x + 1, mainIdx.y) && GetStackedPuyo(mainIdx.x + 1, mainIdx.y) != nullptr))
			{
				mainIdx.x -= 1;
			}
			break;
		case Puyo::ERotation::L:
#if _DEBUG
			OutputDebugStringA("L to B\n");
#endif
			if (!IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y + 2) || (IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y + 2) && GetStackedPuyo(mainIdx.x, mainIdx.y + 2) != nullptr))
			{
				mainIdx.y -= 1;
			}
			break;
		case Puyo::ERotation::R:
#if _DEBUG
			OutputDebugStringA("R to U\n");
#endif
			break;
		default:
			break;
		}
	}

	if (IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y) && GetStackedPuyo(mainIdx.x, mainIdx.y) == nullptr)
	{
		pMainPuyo->Rotate(rotateR);
		UpdateActivePuyo(mainIdx.x, mainIdx.y);
	}
}
void Level_TokoPuyo::ActivePuyoRotateToRelease()
{
	auto idx = pMainPuyo->Get2DIdx();
	if (idx.y >= mGameBoardSize.y)
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
	else if (idx.x >= mGameBoardSize.x)
	{
		idx.x = floor(idx.x);
	}

	UpdateActivePuyo(idx.x, idx.y);
	pGameState->SetGameProgress(*pDX, EGameProgress::Release);
}

// ------------------------------------------------------------
// Main : Release puyo
// ------------------------------------------------------------
void Level_TokoPuyo::DoFrame_Release()
{
	int fallCount = 0;
	if (pMainPuyo->GetRotation() == Puyo::ERotation::B)
	{
		fallCount += DoFrame_Release(pSubPuyo);
		fallCount += DoFrame_Release(pMainPuyo);
	}
	else
	{
		fallCount += DoFrame_Release(pMainPuyo);
		fallCount += DoFrame_Release(pSubPuyo);
	}

	if (fallCount == 0)
	{
		pGameState->SetGameProgress(*pDX, EGameProgress::Wait);
		ActivePuyoReachToBottom();
	}
}
bool Level_TokoPuyo::DoFrame_Release(std::shared_ptr<Puyo>& puyo)
{
	if (puyo->GetIsActive())
	{
		const auto mainIdx = puyo->Get2DIdx();
		if (IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y + 1))
		{
			auto puyoActor = GetStackedPuyo(mainIdx.x, mainIdx.y + 1);
			if (puyoActor == nullptr)
			{
				/* fall */
				//OutputDebugStringA("Stack puyo NAP !!\n");
				SetSpriteLocation(puyo, mainIdx.x, mainIdx.y + 1);
				return true;
			}
			else
			{
				auto puyoActorCurr = GetStackedPuyo(mainIdx.x, mainIdx.y);
				if (puyoActorCurr == nullptr)
				{
					//OutputDebugStringA("Stack puyo + 1!\n");
					GetStackedPuyo(mainIdx.x, mainIdx.y) = puyo;
					GetStackedPuyo(mainIdx.x, mainIdx.y)->SetIsActive(false);
				}
			}
		}
		else
		{
			auto puyoActorCurr = GetStackedPuyo(mainIdx.x, mainIdx.y);
			if (puyoActorCurr == nullptr)
			{
				//OutputDebugStringA(std::format("Stack puyo x:{} y:{} pos:{}\n", puyo.x, puyo.y, CAP).c_str());

				GetStackedPuyo(mainIdx.x, mainIdx.y) = puyo;
				GetStackedPuyo(mainIdx.x, mainIdx.y)->SetIsActive(false);
			}
		}
	}
	return false;
}
void Level_TokoPuyo::ActivePuyoReachToBottom()
{
	OutputDebugStringA("ReachToBottopMainPuyo\n");

	const auto mainIdx = pMainPuyo->Get2DIdx();
	const auto subIdx = pSubPuyo->Get2DIdx();

	int AP = GetPos(mainIdx.x, mainIdx.y);
	int SP = GetPos(subIdx.x, subIdx.y);

	UnionFindPuyo(pMainPuyo);
	UnionFindPuyo(pSubPuyo);

	bool hasVanishPlan = false;
	if (unionFind.size(AP) >= vanishCount)
	{
		hasVanishPlan |= SetPlanToVanishPuyo(pMainPuyo);
	}
	if (unionFind.size(SP) >= vanishCount)
	{
		hasVanishPlan |= SetPlanToVanishPuyo(pSubPuyo);
	}

	pMainPuyo.reset();
	pMainPuyo = nullptr;

	pSubPuyo.reset();
	pSubPuyo = nullptr;

	if (hasVanishPlan)
	{
		pGameState->SetGameProgress(*pDX, EGameProgress::Vanish);
	}
	else
	{
		StartControlPuyo();
	}
}

// ------------------------------------------------------------
// Vanish puyo
// ------------------------------------------------------------
void Level_TokoPuyo::DoFrame_Vanish()
{
	if (pGameState->GetGameProgress() == EGameProgress::Vanish)
	{
		std::chrono::duration Duration = chrono::now() - StartTime_Vanish;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count() >= GameSettings::GetPuyoEffectTime(EPuyoControl::Vanish))
		{
			SetVisibilityVanishPuyo(true);
			VanishPuyo();
			pGameState->SetGameProgress(*pDX, EGameProgress::FallAll);
		}
		else
		{
			FlashVanishPuyo();
		}
	}
}
bool Level_TokoPuyo::SetPlanToVanishPuyo(std::shared_ptr<Puyo> puyo)
{
	//OutputDebugStringA("SetPlanToVanishPuyo\n");
	const auto currIdx = puyo->Get2DIdx();
	const int CP = GetPos(currIdx.x, currIdx.y);
	int P;
	bool hasVanishPlan = false;
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			P = GetPos(x, y);
			if (unionFind.IsSame(CP, P) && unionFind.size(P) >= vanishCount)
			{
				planVanishPuyo[y][x] = true;
				hasVanishPlan = true;
			}
		}
	}
	return hasVanishPlan;
}
bool Level_TokoPuyo::SetPlanToVanishAll()
{
	bool hasVanishPlan = false;
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			if (unionFind.size(GetPos(x, y)) >= vanishCount)
			{
				planVanishPuyo[y][x] = true;
				hasVanishPlan = true;
			}
		}
	}
	return hasVanishPlan;
}
void Level_TokoPuyo::ResetPlanToVanishPuyo()
{
	std::vector<bool> vanishRow(width, false);
	planVanishPuyo.assign(height, vanishRow);
}
void Level_TokoPuyo::FlashVanishPuyo()
{
	VanishPuyoVisibility = !VanishPuyoVisibility;
	SetVisibilityVanishPuyo(VanishPuyoVisibility);
}
void Level_TokoPuyo::SetVisibilityVanishPuyo(bool in)
{
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			if (planVanishPuyo[y][x])
			{
				if (auto puyoActor = GetStackedPuyo(x, y))
				{
					puyoActor->SetTickEnabled(false);
				}
			}
		}
	}
}
void Level_TokoPuyo::VanishPuyo()
{
	std::map<int, int> colors;
	std::map<int, int> rootSize;
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			if (planVanishPuyo[y][x])
			{
				if (auto& puyoActor = GetStackedPuyo(x, y))
				{
					/* Cache for calculate score. */
					++colors[puyoActor->GetType()];
					int P = GetPos(x, y);
					rootSize.emplace(std::make_pair<int, int>(unionFind.root(P), unionFind.size(P)));

					/* Clear puyo. */
					puyoActor.reset();
					puyoActor = nullptr;

					planVanishPuyo[y][x] = false;
				}
			}
		}
	}

	/* calc score */
	int bonus = 0;
	for (auto&& s : rootSize)
	{
		bonus += GameSettings::GetBonusConnect(s.second);
		puyoCount += s.second;
	}
	for (auto&& c : colors)
	{
		if (c.second > 0)
		{
			++colorCount;
		}
	}
	bonus += GameSettings::GetBonusColor(colorCount);
	++comboCount;
	bonus += GameSettings::GetBonusCombo(comboCount);
	bonus = bonus == 0 ? 1 : bonus;

	score += puyoCount * 10 * bonus;

	pGameState->UpdateScore(score, comboCount);

	colorCount = 0;
	puyoCount = 0;

	SE_PuyoVanish->Play();
}

// ------------------------------------------------------------
// Fall puyo
// ------------------------------------------------------------
void Level_TokoPuyo::DoFrame_FallAll()
{
	bool bIsDown = false;
	for (int y = mGameBoardSize.y - 1; y >= 0; --y)
	{
		for (int x = mGameBoardSize.x - 1; x >= 0; --x)
		{
			if (IsValidIndex(stackedPuyo, x, y))
			{
				if (auto& puyoActor = GetStackedPuyo(x, y))
				{
					if (IsValidIndex(stackedPuyo, x, y + 1))
					{
						auto& puyoActorNext = GetStackedPuyo(x, y + 1);
						if (puyoActorNext == nullptr)
						{
							SetSpriteLocation(puyoActor, x, y + 1);
							puyoActorNext = std::move(puyoActor);

							puyoActor.reset();
							puyoActor = nullptr;

							bIsDown = true;
						}
					}
				}
			}
		}
	}
	if (!bIsDown)
	{
		ReachToBottomAll();
	}
}
void Level_TokoPuyo::ReachToBottomAll()
{
	ResetPlanToVanishPuyo();
	RemakeUnionFind();

	if (SetPlanToVanishAll())
	{
		pGameState->SetGameProgress(*pDX, EGameProgress::Vanish);
	}
	else
	{
		ResetPlanToVanishPuyo();
		RemakeUnionFind();

		StartControlPuyo();
	}
}

// ------------------------------------------------------------
// Main : Union Find
// ------------------------------------------------------------
void Level_TokoPuyo::RemakeUnionFind()
{
	unionFind.clear();
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			if (IsValidIndex(stackedPuyo, x, y))
			{
				auto puyoActor = GetStackedPuyo(x, y);
				if (puyoActor != nullptr)
				{
					UnionFindPuyo(puyoActor);
				}
			}
		}
	}
}
void Level_TokoPuyo::UnionFindPuyo(std::shared_ptr<Puyo> puyo)
{
	auto Check = [this, &puyo](uint8_t nx, uint8_t ny)
	{
		if (IsValidIndex(stackedPuyo, nx, ny))
		{
			const auto& puyoActorNext = GetStackedPuyo(nx, ny);
			if (puyoActorNext != nullptr)
			{
				if (puyo->IsSameType(puyoActorNext))
				{
					const auto idx = puyo->Get2DIdx();
					const uint8_t currP = GetPos(idx.x, idx.y);
					const uint8_t nextP = GetPos(nx, ny);

					unionFind.unite(currP, nextP);
				}
			}
		}
	};
	const auto idx = puyo->Get2DIdx();
	Check(idx.x - 1, idx.y);
	Check(idx.x + 1, idx.y);
	Check(idx.x, idx.y - 1);
	Check(idx.x, idx.y + 1);
}

// ------------------------------------------------------------
// Main : Input
// ------------------------------------------------------------
void Level_TokoPuyo::InputUpdate()
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
			SetNeedDurationCached(GameSettings::GetPuyoFallSpeed(EPuyoControl::PressDown));
		}
		else
		{
			ResetNeedDuration();
		}
	}
}

// ------------------------------------------------------
// Main : State : Main Timer
// ------------------------------------------------------
void Level_TokoPuyo::SetNeedDurationCached(DWORD NewVal)
{
	if (pGameState->GetGameProgress() == EGameProgress::Control)
	{
		if (Cached_NeedDurationTime_Main == 0)
		{
			Cached_NeedDurationTime_Main = NeedDurationTime_Main;
			NeedDurationTime_Main = NewVal;
		}
	}
}
void Level_TokoPuyo::ResetNeedDuration()
{
	if (pGameState->GetGameProgress() == EGameProgress::Control)
	{
		if (Cached_NeedDurationTime_Main > 0)
		{
			NeedDurationTime_Main = Cached_NeedDurationTime_Main;
			Cached_NeedDurationTime_Main = 0;
		}
	}
}

// ------------------------------------------------------
// Main : State : Score
// ------------------------------------------------------
void Level_TokoPuyo::ResetCalcScoreCount()
{
	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;
}

// ------------------------------------------------------------
// Main : Utils
// ------------------------------------------------------------
std::shared_ptr<Puyo>& Level_TokoPuyo::GetStackedPuyo(const int& x, const int& y)
{
	if (IsValidIndex(stackedPuyo, x, y))
	{
		return stackedPuyo[round(y)][round(x)];
	}
}
int Level_TokoPuyo::GetPos(uint8_t x, uint8_t y)
{
	// 33 = 5 * 6 + 3
	////OutputDebugStringA(std::format("{} = {} * {} + {}\n", y * mGameBoardSize.x+x, y, mGameBoardSize.x, x).c_str());
	if (x < 0 || x >= mGameBoardSize.x || y < 0 || y >= mGameBoardSize.y)
	{
		return GameSettings::INDEX_NONE;
	}
	return y * mGameBoardSize.x + x;
}
int Level_TokoPuyo::GetPos(FVector2D in)
{
	return GetPos(in.x, in.y);
}
int Level_TokoPuyo::GetPos(float x, float y)
{
	if (x < 0 || x >= mGameBoardSize.x || y < -1 || y >= mGameBoardSize.y)
	{
		return GameSettings::INDEX_NONE;
	}
	return GetPos((uint8_t)ceil(x), (uint8_t)ceil(y));
}
void Level_TokoPuyo::GetXYFromPos(uint8_t pos, uint8_t& x, uint8_t& y)
{
	// x = 33 % 6 = 3;
	x = pos % (uint8_t)mGameBoardSize.y;
	// y = 33 / 6 = 5;
	y = (uint8_t)(pos / (uint8_t)mGameBoardSize.x);
}
uint8_t Level_TokoPuyo::Random()
{
	return Algo::RandRange(0, 3);
}

void Level_TokoPuyo::UpdateActivePuyo(const float& x, const float& y)
{
	SetSpriteLocation(pMainPuyo, x, y);
	UpdateSubPuyoLocationByRotation();
}
void Level_TokoPuyo::UpdateSubPuyoLocationByRotation()
{
	const auto mainIdx = pMainPuyo->Get2DIdx();
	switch (pMainPuyo->GetRotation())
	{
	case Puyo::ERotation::U:
		SetSpriteLocation(pSubPuyo, mainIdx.x, mainIdx.y - 1);
		break;
	case Puyo::ERotation::B:
		SetSpriteLocation(pSubPuyo, mainIdx.x, mainIdx.y + 1);
		break;
	case Puyo::ERotation::L:
		SetSpriteLocation(pSubPuyo, mainIdx.x - 1, mainIdx.y);
		break;
	case Puyo::ERotation::R:
		SetSpriteLocation(pSubPuyo, mainIdx.x + 1, mainIdx.y);
		break;
	default:
		break;
	}
}
