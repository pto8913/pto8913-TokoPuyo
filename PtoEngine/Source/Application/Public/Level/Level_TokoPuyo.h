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

	// ------------------------------------------------------------
	// Main
	// ------------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Level_TokoPuyo" + "#"s + std::to_string(mID);
	}
protected:
	virtual void Init(const int& x, const int& y) override;
	virtual void GenerateGroundLayer() override;

	void Clear();
public:
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void Restart();
	void Pause();
	void Resume();

protected:
	void GameProgressChanged(const EGameProgress& NewState);

	void OnAudioVolumeChanged(float inValue);

	void StartControlPuyo();
	void SpawnPuyo();

	// ---------------------------
	// Main : Control Puyo
	// ---------------------------
	void DoFrame_Control();
	// ---------------------------
	// Main : Puyo Moving
	// ---------------------------
	/*
	@bLeft : true = -1.f, false = 1.f; 
	@rateY : -1.f ~ 1.f 
	*/
	void ActionActivePuyoSlide(float rateX, float rateY);
	void ActivePuyoDownToRelease();

	void ActionActivePuyoRotate(bool rotateR = true);
	void ActivePuyoRotateToRelease();

	// ------------------------------------------------------------
	// Main : Release puyo
	// ------------------------------------------------------------
	/* Fall to Bottom, activePuyo and subPuyo. */
	void DoFrame_Release();
	bool TryToStackPuyo(Puyo* puyoActor);
	/* activePuyo and subPuyo */
	void PuyoIsStacked();
	bool SetPlanToVanishPuyo(Puyo* puyoActor);

	// ------------------------------------------------------------
	// Main : Vanish puyo
	// ------------------------------------------------------------
	void DoFrame_Vanish();
	bool SetPlanToVanishAll();
	void ResetPlanToVanishPuyo();
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
	void UnionFindPuyo(Puyo* puyo);

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

	// ------------------------------------------------------------
	// Debug
	// ------------------------------------------------------------
	void Show() const;
protected:
	// ------------------------------------------------------------
	// Main : Utils
	// ------------------------------------------------------------
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

	virtual void SetSpriteLocation(Actor2D* sprite, const float& worldX, const float& worldY) override;

	void GetXYFromPos(uint8_t pos, uint8_t& x, uint8_t& y);
	uint8_t Random();

	void UpdateActivePuyo(const float& x, const float& y);
	void UpdateSubPuyoLocationByRotation();
private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	FVector2D mGameBoardSize;
	DirectX11* pDX = nullptr;
	GameState_Play* pGameState = nullptr;

	EGameProgress Cached_GameProgress;

	// ----------------------
	// State : Puyo
	// ----------------------
	Puyo* pMainPuyo = nullptr;
	Puyo* pSubPuyo = nullptr;
	uint8_t nextPuyo1_1 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo1_2 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo2_1 = GameSettings::EMPTY_PUYO;
	uint8_t nextPuyo2_2 = GameSettings::EMPTY_PUYO;

	UnionFind unionFind;
	std::vector<std::vector<FPuyoInfos>> stackedPuyo;

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