#pragma once

#include "Object/Actor.h"
#include "Actor/Character/PuyoTypes.h"
#include "GameSettings.h"
#include "Input/Keyboard.h"
#include <chrono>
#include "GameState/GameState_PlayTypes.h"

class Puyo;
class GameState_Play;
class Level_PuyoField;
class Audio;

class Player : public Actor
{
	using chrono = std::chrono::system_clock;
public:
	Player();
	virtual ~Player();

	virtual void NativeOnInitialized(DirectX11& dx) override;

	// -------------------------------------------------------
	// Main
	// -------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Player" + "#"s + std::to_string(mID);
	}
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void Clear();

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

	void UpdateActivePuyo(const float& x, const float& y);
	void UpdateSubPuyoLocationByRotation();
	// ---------------------------
	// Main : State : Main Timer
	// ---------------------------
	void SetNeedDurationCached(DWORD NewVal);
	void ResetNeedDuration();

	// ----------------------
	// Main : Input
	// ----------------------
	void InputUpdate();
protected:
	void OnPaused(bool inPause);

	void OnSpawnPuyo(const uint8_t& mainId, const uint8_t& subId);
	void OnRestart();
	void OnGameProgressChanged(const EGameProgress& NewState);

	// -------------------------------------------------------
	// State
	// -------------------------------------------------------
	GameState_Play* pGameState = nullptr;
	Level_PuyoField* pLevel = nullptr;
	DirectX11* pDX = nullptr;

	// ----------------------
	// State : Puyo
	// ----------------------
	Puyo* pMainPuyo = nullptr;
	Puyo* pSubPuyo = nullptr;

	// ----------------------
	// State : Main Timer
	// ----------------------
	chrono::time_point LastTime_Main;
	std::chrono::milliseconds Duration_Main;
	DWORD NeedDurationTime_Main = 0;
	DWORD Cached_NeedDurationTime_Main = 0;

	// ----------------------
	// State : Play : Sound
	// ----------------------
	std::shared_ptr<Audio> SE_PuyoBottom = nullptr;
	std::shared_ptr<Audio> SE_PuyoMove = nullptr;
	std::shared_ptr<Audio> SE_PuyoRotate = nullptr;

	Keyboard::InputAction InputZ;
	Keyboard::InputAction InputX;
	Keyboard::InputAction InputLeft;
	Keyboard::InputAction InputRight;
	Keyboard::InputAction InputUp;
	Keyboard::InputAxis InputDown;
};