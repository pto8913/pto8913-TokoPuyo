#pragma once

#include "Level2D.h"
#include "GameState/GameState_PlayTypes.h"

#include "Algorithm/UnionFind.h"
#include "Actor/Character/PuyoTypes.h"

#include "GameSettings.h"

#include <chrono>

class Actor2D;
class Puyo;

class Audio;
class GameState_Play;

DECLARE_MULTICAST_DELEGATE(FOnPuyoFieldRestart);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpawnPuyo, const uint8_t&, const uint8_t&);

class Level_PuyoField : public Level2D
{
	using chrono = std::chrono::system_clock;
public:
	Level_PuyoField(DirectX11& dx);
	virtual ~Level_PuyoField();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Level_PuyoField" + "#"s + std::to_string(mID);
	}
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;
protected:
	virtual void Init(const int& x, const int& y) override;
	virtual void GenerateGroundLayer() override;
public:
	// ---------------------------
	// Main : Gameplay
	// ---------------------------
	void OnGameProgressChanged(const EGameProgress& NewState);

	virtual void Clear();

	void Restart();

	void StartControlPuyo();
	void SpawnPuyo();

	// ---------------------------
	// Main : Release puyo
	// ---------------------------
public:
	/* NOTE : Must be call after SetGameProgress(EGameProgress::Release); */
	void SetActivePuyo(Puyo*&& mainPuyoActor, Puyo*&& subPuyoActor);
protected:
	/* Fall to Bottom, activePuyo and subPuyo. */
	void DoFrame_Release();
	bool TryToStackPuyo(Puyo* puyoActor);
	/* activePuyo and subPuyo */
	void PuyoIsStacked();
	bool SetPlanToVanishPuyo(Puyo* puyoActor);

	// ---------------------------
	// Main : Vanish puyo
	// ---------------------------
	void DoFrame_Vanish();
	bool SetPlanToVanishAll();
	void ResetPlanToVanishPuyo();
	void VanishPuyo();
	bool VanishPuyoVisibility = true;

	// ---------------------------
	// Main : Fall puyo
	// ---------------------------
	void DoFrame_FallAll();
	void ReachToBottomAll();

	// ---------------------------
	// Main : Union Find
	// ---------------------------
	void RemakeUnionFind();
	void UnionFindPuyo(Puyo* puyo);

public:
	// ---------------------------
	// Main : State : Main Timer
	// ---------------------------
	void SetNeedDurationCached(DWORD NewVal);
	void ResetNeedDuration();

protected:
	// ---------------------------
	// Main : State : Score
	// ---------------------------
	void ResetCalcScoreCount();

public:
	// ---------------------------
	// Debug
	// ---------------------------
	void Show() const;

protected:
	// ---------------------------
	// Main : Utils
	// ---------------------------
	template<typename T>
	bool IsValidIndex(const std::vector<T>& vector, const int& Idx) const
	{
		return vector.size() > Idx;
	}

	template<typename T>
	bool IsValidIndex(const std::vector<std::vector<T>>& vec, const int& x, const int& y) const
	{
		if (IsValidIndex(vec, round(y)))
		{
			return IsValidIndex(vec[round(y)], round(x));
		}
		return false;
	}

	int GetPos(float x, float y);
	int GetPos(FVector2D in);

	FPuyoInfos& GetStackedPuyo(const int& x, const int& y);

	void GetXYFromPos(uint8_t pos, uint8_t& x, uint8_t& y);
	uint8_t Random();
public:
	virtual void SetSpriteLocation(Actor2D* sprite, const float& worldX, const float& worldY) override;
	bool IsValidIndex(const int& x, const int& y) const;
	bool IsValidStackedPuyo(const int& x, const int& y) const;

	// ---------------------------
	// Main : Delegates
	// ---------------------------
	FOnPuyoFieldRestart OnPuyoFieldRestart;
	FOnSpawnPuyo OnSpawnPuyo;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------

	// -----------------------------------
	// State : Gameplay
	// -----------------------------------
	FVector2D mGameBoardSize;
	DirectX11* pDX = nullptr;
	GameState_Play* pGameState = nullptr;

	// ----------------------
	// State : Play : Puyo
	// ----------------------
	UnionFind unionFind;
	std::vector<std::vector<FPuyoInfos>> stackedPuyo;
	std::vector<std::vector<bool>> planVanishPuyo;

	Puyo* pMainPuyo = nullptr;
	Puyo* pSubPuyo = nullptr;
	uint8_t nextPuyo1_1 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo1_2 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo2_1 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo2_2 = GameSettings::EMPTY_PUYO;

	// ----------------------
	// State : Main Timer
	// ----------------------
	chrono::time_point LastTime_Main;
	std::chrono::milliseconds Duration_Main;
	DWORD NeedDurationTime_Main;
	DWORD Cached_NeedDurationTime_Main;

	// ----------------------
	// State : Vanish
	// ----------------------
	chrono::time_point StartTime_Vanish;

	// ----------------------
	// State : Play : Sound
	// ----------------------
	std::shared_ptr<Audio> SE_PuyoVanish = nullptr;
	std::shared_ptr<Audio> SE_PuyoGameOver = nullptr;

	// ----------------------
	// State : Score
	// ----------------------
	uint8_t vanishCount = 4;
	int puyoCount = 0;
	int comboCount = 0;
	int connectCount = 0;
	int colorCount = 0;
	int score = 0;
};