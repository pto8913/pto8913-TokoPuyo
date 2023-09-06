#pragma once

#include "Level2D.h"
#include "Algorithm/UnionFind.h"

class DirectX11;
class Audio;
class Sprite;

class Level_TokoPuyo : public Level2D
{
public:
	Level_TokoPuyo(DirectX11& dx);
	virtual ~Level_TokoPuyo();

	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

protected:
	virtual void GenerateGroundLayer() override;
	virtual void GenerateEventLayer() override;
	virtual void GenerateCharacterLayer() override;
	virtual void GenerateEffectLayer() override;

private:
	void StartControlPuyo();
	void SpawnPuyo();

	// ------------------------------------------------------------
	// Main : UI
	// ------------------------------------------------------------
	void OnClickedRestart(DX::MouseEvent inMouseEvent);
	void OnClickedPause(DX::MouseEvent inMouseEvent);

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
	bool DoFrame_Release(Puyo& puyo, std::shared_ptr<Sprite> pPuyo);
	/* activePuyo and subPuyo */
	void ReachToBottomActivePuyo();

	// ------------------------------------------------------------
	// Main : Vanish puyo
	// ------------------------------------------------------------
	void DoFrame_Vanish();
	bool SetPlanToVanishPuyo(Puyo puyo);
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
	void UnionFindPuyo(Puyo puyo);

	// ------------------------------------------------------------
	// Main : Input
	// ------------------------------------------------------------
	void InputUpdate();
	void OnClickedDown(DX::MouseEvent inMouseEvent);
	void OnClickedHeld(DX::MouseEvent inMouseEvent);
	void OnClickedUp(DX::MouseEvent inMouseEvent);
	void OnMouseMove(DX::MouseEvent inMouseEvent);

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
	// Main : Utils
	// ------------------------------------------------------------
	template<typename T>
	bool IsValidIndex(const std::vector<T>& vector, const int& Idx)
	{
		return vector.size() > Idx;
	}
	bool IsValidIndex(const int& Idx)
	{
		//if (Idx < 0)
		//{
		//	if (Idx > -6)
		//	{
		//		return true;
		//	}
		//}
		return stackedPuyo.size() > Idx;
	}

	int GetPos(UINT8 x, UINT8 y);
	int GetPos(float x, float y);
	int GetPos(DirectX::XMFLOAT2 in);

	void GetXYFromPos(UINT8 pos, UINT8& x, UINT8& y);


private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	UINT8 size;
	DirectX11* m_pdx;

	std::shared_ptr<Sprite> BackGround;
	std::shared_ptr<DX::IControllerInterface> ControllerInterface;

	// ----------------------
	// State : UI
	// ----------------------

	GameStateUI* m_pGameStateUI;
	DX::GameState Cached_GameState;

	// ----------------------
	// State : Puyo
	// ----------------------
	std::mt19937 gen;
	std::uniform_int_distribution<int> distr;

	Puyo activePuyo;
	Puyo subPuyo;
	UINT8 subId;
	std::shared_ptr<Sprite> m_pActivePuyo;
	std::shared_ptr<Sprite> m_pSubPuyo;
	UINT8 nextPuyo1_1 = Config::EMPTY_PUYO;
	UINT8 nextPuyo1_2 = Config::EMPTY_PUYO;
	UINT8 nextPuyo2_1 = Config::EMPTY_PUYO;
	UINT8 nextPuyo2_2 = Config::EMPTY_PUYO;

	UnionFind unionFind;
	std::vector<Puyo> stackedPuyo;
	std::vector<std::shared_ptr<Sprite>> stackedPuyoSprites;

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
	std::vector<bool> planVanishPuyo;

	// ----------------------
	// State : Score
	// ----------------------
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
};