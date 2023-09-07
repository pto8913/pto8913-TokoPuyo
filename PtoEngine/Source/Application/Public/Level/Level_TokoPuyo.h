#pragma once

#include "Level2D.h"
#include "Algorithm/UnionFind.h"
#include "Actor/Character/PuyoTypes.h"
#include "GameState/GameState_PlayTypes.h"
#include "GameSettings.h"
#include "Input/Keyboard.h"

class DirectX11;
class Audio;

class Puyo;

class GameState_Play;

class Level_TokoPuyo : public Level2D
{
	using chrono = std::chrono::system_clock;
public:
	Level_TokoPuyo(DirectX11& dx);
	virtual ~Level_TokoPuyo();
protected:
	virtual void GenerateGroundLayer() override;
	virtual void GenerateEventLayer() override;
	virtual void GenerateCharacterLayer() override;
	virtual void GenerateEffectLayer() override;
public:
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void Restart();
	void Pause();

protected:
	void GameProgressChanged(const EGameProgress& NewState);

	void StartControlPuyo();
	void SpawnPuyo();


	// ------------------------------------------------------------
	// Main : Control Puyo
	// ------------------------------------------------------------
	void DoFrame_Control();
	// ---------------------------
	// Main : Puyo Moving
	// ---------------------------
	void ActionActivePuyoDown(float rate);
	void ActivePuyoDownToRelease();
	void ActionActivePuyoSlide(bool left);
	void ActionActivePuyoRotate(bool rotateR = true);
	void ActivePuyoRotateToRelease();

	// ------------------------------------------------------------
	// Main : Release puyo
	// ------------------------------------------------------------
	/* Fall to Bottom, activePuyo and subPuyo. */
	void DoFrame_Release();
	bool DoFrame_Release(std::shared_ptr<Puyo>& puyo);
	/* activePuyo and subPuyo */
	void ReachToBottomMainPuyo();

	// ------------------------------------------------------------
	// Main : Vanish puyo
	// ------------------------------------------------------------
	void DoFrame_Vanish();
	bool SetPlanToVanishPuyo(std::shared_ptr<Puyo> puyo);
	bool SetPlanToVanishAll();
	void ResetPlanToVanishPuyo();
	void FlashVanishPuyo();
	void SetVisibilityVanishPuyo(bool in);
	void VanishPuyo();
	bool VanishPuyoVisibility = true;

	// ------------------------------------------------------------
	// Main : Fall puyo
	// ------------------------------------------------------------
	void DoFrame_FallAll();
	void ReachToBottomAll();

	// ------------------------------------------------------------
	// Main : Union Find
	// ------------------------------------------------------------
	void RemakeUnionFind();
	void UnionFindPuyo(std::shared_ptr<Puyo> puyo);

	// ------------------------------------------------------------
	// Main : Input
	// ------------------------------------------------------------
	void InputUpdate();

	// ------------------------------------------------------------
	// Main : State : Main Timer
	// ------------------------------------------------------------
	void SetNeedDurationCached(DWORD NewVal);
	void ResetNeedDuration();

	// ------------------------------------------------------------
	// Main : State : Score
	// ------------------------------------------------------------
	void ResetCalcScoreCount();

protected:
	// ------------------------------------------------------------
	// Main : Utils
	// ------------------------------------------------------------
	template<typename T>
	bool IsValidIndex(const std::vector<T>& vector, const int& Idx)
	{
		return vector.size() > Idx;
	}

	template<typename T>
	bool IsValidIndex(const std::vector<std::vector<T>>& vec, const int& x, const int& y)
	{
		if (IsValidIndex(vec, y))
		{
			return IsValidIndex(vec[y], x);
		}
		return false;
	}

	int GetPos(uint8_t x, uint8_t y);
	int GetPos(float x, float y);
	int GetPos(FVector2D in);

	void GetXYFromPos(uint8_t pos, uint8_t& x, uint8_t& y);
	uint8_t Random();

	void UpdateSubPuyoLocationByRotation();
private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	uint8_t size;

	FVector2D mGameBoardSize;
	DirectX11* pDX = nullptr;

	EGameProgress Cached_GameProgress;

	// ----------------------
	// State : UI
	// ----------------------
	std::shared_ptr<GameState_Play> pGameState = nullptr;

	// ----------------------
	// State : Puyo
	// ----------------------
	uint8_t subPuyoId;
	std::shared_ptr<Puyo> pMainPuyo;
	std::shared_ptr<Puyo> pSubPuyo;
	uint8_t nextPuyo1_1 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo1_2 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo2_1 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo2_2 = GameSettings::EMPTY_PUYO;

	UnionFind unionFind;
	std::vector<std::vector<std::shared_ptr<Puyo>>> stackedPuyo;

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
	chrono::duration DurationTime_Vanishn;
	std::vector<std::vector<bool>> planVanishPuyo;

	// ----------------------
	// State : Score
	// ----------------------
	uint8_t vanishCount = 4;
	int puyoCount = 0;
	int comboCount = 0;
	int connectCount = 0;
	int colorCount = 0;
	int score = 0;

	// ----------------------
	// State : Sound
	// ----------------------
	std::shared_ptr<Audio> BGM = nullptr;
	std::shared_ptr<Audio> SE_PuyoMove = nullptr;
	std::shared_ptr<Audio> SE_PuyoBottom = nullptr;
	std::shared_ptr<Audio> SE_PuyoRotate = nullptr;
	std::shared_ptr<Audio> SE_PuyoVanish = nullptr;
	std::shared_ptr<Audio> SE_PuyoGameOver = nullptr;

	Keyboard::InputAction InputZ;
	Keyboard::InputAction InputX;
	Keyboard::InputAction InputLeft;
	Keyboard::InputAction InputRight;
	Keyboard::InputAction InputUp;
	Keyboard::InputAxis InputDown;
};