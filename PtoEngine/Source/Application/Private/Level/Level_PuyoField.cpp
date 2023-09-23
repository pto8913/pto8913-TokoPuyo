
#include "Level/Level_PuyoField.h"

#include "Framework/World.h"
#include "GameState/GameState_Play.h"

#include "Engine/Audio.h"

#include "Actor/Actor2D.h"
#include "Actor/Character/Puyo.h"
#include "Actor/Ground/GroundBase.h"

#include "PtoGameInstance.h"

Level_PuyoField::Level_PuyoField(DirectX11& dx)
	: Level2D(dx)
{
	pDX = &dx;

	const auto masterVolume = GameSettings::GetAudioVolume();
	SE_PuyoVanish = std::make_shared<Audio>(L"Content/Sounds/Puyo_Vanish.wav");
	SE_PuyoVanish->SetVolume(0.25f);
	SE_PuyoVanish->SetVolume(SE_PuyoVanish->GetDefaultVolume() * masterVolume / 10.f);

	SE_PuyoGameOver = std::make_shared<Audio>(L"Content/Sounds/Puyo_GameOver.wav");
	SE_PuyoGameOver->SetVolume(SE_PuyoGameOver->GetDefaultVolume() * masterVolume / 10.f);

	mGameBoardSize = GameSettings::GetGameBoardSize();
	Init(mGameBoardSize.x, mGameBoardSize.y);

	Cached_NeedDurationTime_Main = 0;
	LastTime_Main = chrono::now();
}
Level_PuyoField::~Level_PuyoField()
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
	Stop(SE_PuyoVanish);
	Stop(SE_PuyoGameOver);
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level_PuyoField::BeginPlay(DirectX11& dx)
{
	Level2D::BeginPlay(dx);

	pGameState = static_cast<GameState_Play*>(GetWorld()->GetGameState());
	pGameState->OnGameProgressChanged.Bind<&Level_PuyoField::OnGameProgressChanged>(*this, GetName());
	pGameState->OnRestart.Bind<&Level_PuyoField::Restart>(*this, GetName());
	OnGameProgressChanged(pGameState->GetGameProgress());
}
void Level_PuyoField::Tick(DirectX11& dx, float deltaTime)
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
			case EGameProgress::Release:
				DoFrame_Release();
				break;
			case EGameProgress::Vanish:
				DoFrame_Vanish();
				break;
			case EGameProgress::FallAll:
				DoFrame_FallAll();
				break;
			default:
				break;
			}
		}
	}
}
void Level_PuyoField::Init(const int& x, const int& y)
{
	Level2D::Init(x, y);

	const auto size = (int)(x * y);
	unionFind = UnionFind(size);
	stackedPuyo.resize(size);
	planVanishPuyo.resize(size);
}
void Level_PuyoField::GenerateGroundLayer()
{
	auto world = GetWorld();
	for (int y = 0; y < height - 1; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			auto ground = world->SpawnActor<GroundBase>(*pDX, EGroundId::Background);
			SetSpriteLocation(ground, x, y + 1);
			ground = nullptr;
		}
	}
}

// ---------------------------
// Main : Gameplay
// ---------------------------
void Level_PuyoField::OnGameProgressChanged(const EGameProgress& NewState)
{
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
		PtoGameInstance::Get().SetBGM(false);
		SE_PuyoGameOver->Play();
		break;
	default:
		NeedDurationTime_Main = GameSettings::GetPuyoFallSpeed(EPuyoControl::Wait);
		break;
	}
}

void Level_PuyoField::Clear()
{
	for (auto&& layer : GetObjectManager()->pObjects)
	{
		auto iter = layer.second.begin();
		while (iter != layer.second.end())
		{
			auto& obj = *iter;
			if (obj != nullptr)
			{
				if (obj->HasTag(GameSettings::PUYO_TAG))
				{
					obj->MarkPendingKill();
				}
			}
			++iter;
		}
	}

	unionFind.clear();

	for (auto& row : stackedPuyo)
	{
		row.erase(row.begin(), row.end());
	}
	stackedPuyo.clear();

	planVanishPuyo.erase(planVanishPuyo.begin(), planVanishPuyo.end());
	planVanishPuyo.clear();

	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;

	nextPuyo1_1 = GameSettings::EMPTY_PUYO;
	nextPuyo1_2 = GameSettings::EMPTY_PUYO;
	nextPuyo2_1 = GameSettings::EMPTY_PUYO;
	nextPuyo2_2 = GameSettings::EMPTY_PUYO;
}

void Level_PuyoField::Restart()
{
	pGameState->SetGameProgress(EGameProgress::Wait);

	Clear();

	std::vector<FPuyoInfos> puyoRow(mGameBoardSize.x, FPuyoInfos(-1));
	stackedPuyo.assign(mGameBoardSize.y, puyoRow);

	LastTime_Main = chrono::now();

	ResetPlanToVanishPuyo();

	pGameState->UpdateScore(0, 0);

	StartControlPuyo();
}

void Level_PuyoField::StartControlPuyo()
{
#if _DEBUG
	OutputDebugStringA("Restart Puyo\n");
#endif
	const auto idx = GameSettings::GetGameOverCoord();
	if (IsValidIndex(stackedPuyo, idx.x, idx.y))
	{
		if (GetStackedPuyo(idx.x, idx.y))
		{
			pGameState->SetGameProgress(EGameProgress::GameOver);
		}
		else
		{
			pGameState->SetGameProgress(EGameProgress::Control);

			ResetCalcScoreCount();
			SpawnPuyo();
		}
	}
	else
	{
		pGameState->SetGameProgress(EGameProgress::GameOver);
	}
}
void Level_PuyoField::SpawnPuyo()
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

	OnSpawnPuyo.Broadcast(mainPuyoId, subPuyoId);

	pGameState->UpdateNextPuyo(nextPuyo1_1, nextPuyo1_2, nextPuyo2_1, nextPuyo2_2);
}

// ------------------------------------------------------------
// Main : Release puyo
// ------------------------------------------------------------
void Level_PuyoField::SetActivePuyo(Puyo*&& mainPuyoActor, Puyo*&& subPuyoActor)
{
	pMainPuyo = std::move(mainPuyoActor);
	pSubPuyo = std::move(subPuyoActor);
}
void Level_PuyoField::DoFrame_Release()
{
	if (pMainPuyo != nullptr)
	{
		int fallCount = 0;
		if (pMainPuyo->GetPuyoInfos().GetRotation() == FPuyoInfos::ERotation::B)
		{
			fallCount += TryToStackPuyo(pSubPuyo);
			fallCount += TryToStackPuyo(pMainPuyo);
		}
		else
		{
			fallCount += TryToStackPuyo(pMainPuyo);
			fallCount += TryToStackPuyo(pSubPuyo);
		}

		if (fallCount == 0)
		{
			pGameState->SetGameProgress(EGameProgress::Wait);
			PuyoIsStacked();
#if _DEBUG
			Show();
#endif
		}
	}
}
bool Level_PuyoField::TryToStackPuyo(Puyo* puyoActor)
{
	auto& currPuyoInfos = puyoActor->GetPuyoInfos();
	if (currPuyoInfos.GetIsActive())
	{
		const auto mainIdx = puyoActor->Get2DIdx();
		if (IsValidIndex(stackedPuyo, mainIdx.x, mainIdx.y + 1))
		{
			auto nextPuyoInfos = GetStackedPuyo(mainIdx.x, mainIdx.y + 1);
			if (!nextPuyoInfos)
			{
				/* fall */
				SetSpriteLocation(puyoActor, mainIdx.x, mainIdx.y + 1);
				return true;
			}
			else
			{
				/* stack puyo */
				currPuyoInfos.SetIsActive(false);
				stackedPuyo[mainIdx.y][mainIdx.x] = currPuyoInfos;
			}
		}
		else
		{
			/* stack puyo */
			currPuyoInfos.SetIsActive(false);
			stackedPuyo[mainIdx.y][mainIdx.x] = currPuyoInfos;
		}
	}
	return false;
}
void Level_PuyoField::PuyoIsStacked()
{
#if _DEBUG
	OutputDebugStringA("ReachToBottopMainPuyo\n");
#endif
	const auto mainIdx = pMainPuyo->Get2DIdx();
	const auto subIdx = pSubPuyo->Get2DIdx();

	UnionFindPuyo(pMainPuyo);
	UnionFindPuyo(pSubPuyo);

	bool hasVanishPlan = false;
	if (unionFind.size(GetPos(mainIdx)) >= vanishCount)
	{
		hasVanishPlan |= SetPlanToVanishPuyo(pMainPuyo);
	}
	if (unionFind.size(GetPos(subIdx)) >= vanishCount)
	{
		hasVanishPlan |= SetPlanToVanishPuyo(pSubPuyo);
	}

	pMainPuyo = nullptr;
	pSubPuyo = nullptr;

	if (hasVanishPlan)
	{
		pGameState->SetGameProgress(EGameProgress::Vanish);
	}
	else
	{
		StartControlPuyo();
	}
}
bool Level_PuyoField::SetPlanToVanishPuyo(Puyo* puyoActor)
{
	const auto currPos = GetPos(puyoActor->Get2DIdx());
	bool hasVanishPlan = false;
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			const auto& Pos = GetPos(x, y);
			if (unionFind.IsSame(currPos, Pos) && unionFind.size(Pos) >= vanishCount)
			{
				planVanishPuyo[y][x] = true;
				hasVanishPlan = true;
			}
		}
	}
	return hasVanishPlan;
}

// ------------------------------------------------------------
// Vanish puyo
// ------------------------------------------------------------
void Level_PuyoField::DoFrame_Vanish()
{
	if (pGameState->GetGameProgress() == EGameProgress::Vanish)
	{
#if _DEBUG
		OutputDebugStringA("Vanish Loop\n");
#endif
		chrono::duration Duration = chrono::now() - StartTime_Vanish;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count() >= GameSettings::GetPuyoEffectTime(EPuyoControl::Vanish))
		{
			VanishPuyo();
			pGameState->SetGameProgress(EGameProgress::FallAll);
		}
	}
}
bool Level_PuyoField::SetPlanToVanishAll()
{
	bool hasVanishPlan = false;
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			const auto ID = stackedPuyo[y][x].GetID();
			if (ID != -1)
			{
				auto puyoObj = GetObjectManager()->GetObjectByID(ID);
				auto puyoActor = static_cast<Actor2D*>(puyoObj);
				if (unionFind.size(GetPos(puyoActor->Get2DIdx())) >= vanishCount)
				{
					planVanishPuyo[y][x] = true;
					hasVanishPlan = true;
				}
			}
		}
	}
	return hasVanishPlan;
}
void Level_PuyoField::ResetPlanToVanishPuyo()
{
	std::vector<bool> vanishRow(mGameBoardSize.x, false);
	planVanishPuyo.assign(mGameBoardSize.y, vanishRow);
}

void Level_PuyoField::VanishPuyo()
{
#if _DEBUG
	OutputDebugStringA("Vanish\n");
#endif
	std::map<int, int> colors;
	std::map<int, int> rootSize;

	auto& objects = GetObjectManager()->pObjects[Layer::EActorLayer::Entities];
	auto iter = objects.begin();
	while (iter != objects.end())
	{
		auto& puyoObj = *iter;
		if (puyoObj != nullptr)
		{
			if (puyoObj->HasTag(GameSettings::PUYO_TAG))
			{
				auto puyoActor = static_pointer_cast<Puyo>(puyoObj);
				if (puyoActor != nullptr)
				{
					const auto idx = puyoActor->Get2DIdx();
					if (planVanishPuyo[idx.y][idx.x])
					{
						/* Cache for calculate score. */
						++colors[puyoActor->GetPuyoInfos().GetType()];
						const auto& pos = GetPos(puyoActor->Get2DIdx());
						rootSize.emplace(std::make_pair<int, int>(unionFind.root(pos), unionFind.size(pos)));

						/* Clear puyo. */
						puyoObj->MarkPendingKill();

						stackedPuyo[idx.y][idx.x] = FPuyoInfos(-1);
						planVanishPuyo[idx.y][idx.x] = false;
					}
				}
			}
		}
		++iter;
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

	pGameState->SetAllClear(false);
}

// ------------------------------------------------------------
// Fall puyo
// ------------------------------------------------------------
void Level_PuyoField::DoFrame_FallAll()
{
	bool bIsDown = false;
	bool bIsPuyoExists = false;
	auto& objects = GetObjectManager()->pObjects[Layer::EActorLayer::Entities];
	auto iter = objects.begin();
	while (iter != objects.end())
	{
		auto& puyoObj = *iter;
		if (puyoObj != nullptr)
		{
			if (puyoObj->HasTag(GameSettings::PUYO_TAG))
			{
				bIsPuyoExists = true;
				auto puyoActor = static_pointer_cast<Actor2D>(puyoObj);
				if (puyoActor != nullptr)
				{
					auto idx = puyoActor->Get2DIdx();
					if (IsValidIndex(stackedPuyo, idx.x, idx.y + 1))
					{
						if (!stackedPuyo[idx.y + 1][idx.x])
						{
							/* Update pre puyo infos */
							auto prePuyoInfos = std::move(stackedPuyo[idx.y][idx.x]);
							stackedPuyo[idx.y][idx.x] = FPuyoInfos(-1);

							++idx.y;
							/* Update next puyo infos */
							SetSpriteLocation(puyoActor.get(), idx.x, idx.y);
							stackedPuyo[idx.y][idx.x] = std::move(prePuyoInfos);

							bIsDown = true;
						}
					}
				}
			}
		}
		++iter;
	}

	if (!bIsPuyoExists)
	{
		pGameState->SetAllClear(true);
	}
	if (!bIsDown)
	{
		ReachToBottomAll();
	}
}
void Level_PuyoField::ReachToBottomAll()
{
	ResetPlanToVanishPuyo();
	RemakeUnionFind();

	if (SetPlanToVanishAll())
	{
		pGameState->SetGameProgress(EGameProgress::Vanish);
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
void Level_PuyoField::RemakeUnionFind()
{
	unionFind.clear();
	for (uint8_t y = 0; y < mGameBoardSize.y; ++y)
	{
		for (uint8_t x = 0; x < mGameBoardSize.x; ++x)
		{
			if (IsValidIndex(stackedPuyo, x, y))
			{
				auto& puyoInfos = GetStackedPuyo(x, y);
				if (puyoInfos.IsValid())
				{
					auto puyoObj = GetObjectManager()->GetObjectByID(puyoInfos.GetID());
					auto puyoActor = static_cast<Puyo*>(puyoObj);
					UnionFindPuyo(puyoActor);
				}
			}
		}
	}
}
void Level_PuyoField::UnionFindPuyo(Puyo* puyo)
{
#if _DEBUG
	OutputDebugStringA("------------------- UnionFind ----------------- \n");
#endif
	auto Check = [this, &puyo](uint8_t nx, uint8_t ny)
	{
		if (IsValidIndex(stackedPuyo, nx, ny))
		{
			const auto& puyoActorNext = stackedPuyo[ny][nx];
			if (puyoActorNext.IsValid())
			{
#if _DEBUG
				OutputDebugStringA(std::format("puyo curr {}, next {}\n", puyo->GetPuyoInfos().GetType(), puyoActorNext.GetType()).c_str());
#endif
				if (puyo->GetPuyoInfos().IsSameType(puyoActorNext))
				{
					const auto nextID = puyoActorNext.GetID();
#if _DEBUG
					OutputDebugStringA(std::format("same type \n").c_str());
					OutputDebugStringA(std::format("nextPuyo ID {}\n", nextID).c_str());
#endif
					if (nextID != -1)
					{
						auto nextPuyoObj = GetObjectManager()->GetObjectByID(nextID);
						auto nextPuyoActor = static_cast<Actor2D*>(nextPuyoObj);
#if _DEBUG
						OutputDebugStringA(std::format("unite {}, {}\n", GetPos(puyo->Get2DIdx()), GetPos(nextPuyoActor->Get2DIdx())).c_str());
#endif
						unionFind.unite(GetPos(puyo->Get2DIdx()), GetPos(nextPuyoActor->Get2DIdx()));
					}
				}
			}
		}
	};
	const auto idx = puyo->Get2DIdx();
	Check(idx.x - 1, idx.y);
	Check(idx.x + 1, idx.y);
	Check(idx.x, idx.y - 1);
	Check(idx.x, idx.y + 1);
#if _DEBUG
	OutputDebugStringA("----------------------------------------------- \n");
#endif
}

// ------------------------------------------------------
// Main : State : Main Timer
// ------------------------------------------------------
void Level_PuyoField::SetNeedDurationCached(DWORD NewVal)
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
void Level_PuyoField::ResetNeedDuration()
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
void Level_PuyoField::ResetCalcScoreCount()
{
	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;
}

// ------------------------------------------------------------
// Debug
// ------------------------------------------------------------
void Level_PuyoField::Show() const
{
#if _DEBUG
	OutputDebugStringA("------------------- Show ----------------- \n");
	for (int y = 0; y < mGameBoardSize.y; ++y)
	{
		std::string str = "";
		for (int x = 0; x < mGameBoardSize.x; ++x)
		{
			if (IsValidIndex(stackedPuyo, x, y))
			{
				switch (stackedPuyo[y][x].GetType())
				{
				case 0:
					str += "R";
					break;
				case 1:
					str += "G";
					break;
				case 2:
					str += "Y";
					break;
				case 3:
					str += "P";
					break;
				default:
					str += " ";
					break;
				}
			}
		}
		OutputDebugStringA((str + "\n").c_str());
	}
	OutputDebugStringA("------------------- UniF ----------------- \n");

	std::vector<int> parents;
	unionFind.GetParent(parents);
	for (auto p : parents)
	{
		OutputDebugStringA((std::to_string(p) + " ").c_str());
	}
	OutputDebugStringA("\n");
	OutputDebugStringA("------------------------------------------ \n");
#endif
}

// ------------------------------------------------------------
// Main : Utils
// ------------------------------------------------------------
int Level_PuyoField::GetPos(FVector2D in)
{
	return GetPos(in.x, in.y);
}
int Level_PuyoField::GetPos(float x, float y)
{
	if (x < 0 || x >= mGameBoardSize.x || y < -1 || y >= mGameBoardSize.y)
	{
		return GameSettings::INDEX_NONE;
	}
	return ceil(y) * mGameBoardSize.x + ceil(x);
}

FPuyoInfos& Level_PuyoField::GetStackedPuyo(const int& x, const int& y)
{
	if (IsValidIndex(stackedPuyo, x, y))
	{
		return stackedPuyo[round(y)][round(x)];
	}
	FPuyoInfos out(-1);
	return out;
}

void Level_PuyoField::GetXYFromPos(uint8_t pos, uint8_t& x, uint8_t& y)
{
	// x = 33 % 6 = 3;
	x = pos % (uint8_t)mGameBoardSize.y;
	// y = 33 / 6 = 5;
	y = (uint8_t)(pos / (uint8_t)mGameBoardSize.x);
}
uint8_t Level_PuyoField::Random()
{
	return Algo::RandRange(0, 3);
}

void Level_PuyoField::SetSpriteLocation(Actor2D* sprite, const float& worldX, const float& worldY)
{
	DirectX::XMFLOAT2 pos = WorldToScreen(worldX, worldY - 1, sprite->GetActorScale());

	sprite->SetActorLocation(FVector(pos.x, pos.y, 0.f));
	sprite->Set2DIdx(FVector2D(worldX, worldY));
}
bool Level_PuyoField::IsValidIndex(const int& x, const int& y) const
{
	return IsValidIndex(stackedPuyo, x, y);
}
bool Level_PuyoField::IsValidStackedPuyo(const int& x, const int& y) const
{
	if (IsValidIndex(stackedPuyo, x, y))
	{
		return stackedPuyo[round(y)][round(x)].IsValid();
	}
	return false;
}