#pragma once

#include "DirectX/DirectXHead.h"

#include "GameUIController.h"
#include <chrono>

#include "Input/Keyboard.h"
#include "Input/ControllerInterface.h"

#include "Slate/SlateInterface.h"

#include "Config.h"

class DirectX11;

using chrono = std::chrono::system_clock;

class Sprite;
class GameMode;
class GameStateUI;

struct UnionFind
{
	std::vector<int> _parent, _rank, _size;
public:
	UnionFind() = default;
	UnionFind(int n) : _parent(n, -1), _rank(n, 0), _size(n, 1), N(n) {}

	void clear()
	{
		_parent.clear();
		_rank.clear();
		_size.clear();
		_parent.assign(N, -1);
		_rank.assign(N, 0);
		_size.assign(N, 1);
	}

	int root(int x)
	{
		if (_parent[x] == -1)
		{
			return x;
		}
		else
		{
			return _parent[x] = root(_parent[x]);
		}
	}

	bool isSame(int x, int y)
	{
		return root(x) == root(y);
	}

	bool unite(int x, int y)
	{
		int root_x = root(x);
		int root_y = root(y);
		if (root_x == root_y)
		{
			return false;
		}
		if (_rank[root_x] < _rank[root_y])
		{
			std::swap(root_x, root_y);
		}
		_parent[root_y] = root_x;
		if (_rank[root_x] == _rank[root_y])
		{
			++_rank[root_x];
		}
		_size[root_x] += _size[root_y];
		return true;
	}
	int size(int x)
	{
		return _size[root(x)];
	}
private:
	int N;
};

struct Puyo
{
public:
	Puyo() : Id(Config::EMPTY_PUYO), IsActive(true), rotation(Rotation::U){}
	Puyo(UINT8 inId) : Id(inId), IsActive(true), rotation(Rotation::U) {}

	void CopyToSub(Puyo& target)
	{
		target.rotation = rotation;
		target.x = x;
		target.y = y;
		switch (rotation)
		{
		case Rotation::U:
			target.y = y - 1;
			break;
		case Rotation::R:
			target.x = x + 1;
			break;
		case Rotation::B:
			target.y = y + 1;
			break;
		case Rotation::L:
			target.x = x - 1;
			break;
		default:
			break;
		}
		target.UpdateOffset();
	}

	bool IsSame(const Puyo& In) const
	{
		return Id == In.Id && x == In.x && y == In.y;
	}
	bool IsSameId(const Puyo& In) const
	{
		return Id == In.Id;
	}
	bool IsEmpty() const
	{
		return Id == Config::EMPTY_PUYO;
	}

	void SetEmpty()
	{
		Id = Config::EMPTY_PUYO;
		IsActive = true;
		rotation = Rotation::U;
		offset = { 0, 0 };
		x, y = 0;
	}

	void UpdateOffset(DirectX::XMFLOAT2 Additional = {0, 0})
	{
		offset.x = Config::GAMESCREEN_PADDING.x + x * Config::CELL + Additional.x;
		offset.y = Config::GAMESCREEN_PADDING.y + (y - 1) * Config::CELL + Additional.y;
	}
	
	/* 0 ~ 3 */
	UINT8 Id;
	bool IsActive;

	/* x : 0 ~W - 1 */
	float x;
	/* y: 0 ~H - 1 */
	float y;

	DirectX::XMFLOAT2 offset;

	enum class Rotation : UINT8
	{
		U = 0,
		R = 1,
		B = 2,
		L = 3,
	};
	
	Rotation rotation;
	void Rotate(bool rotateR)
	{
		int r = static_cast<int>(rotation);
		if (rotateR)
		{
			wrap_rotation(++r);
		}
		else
		{
			wrap_rotation(--r);
		}
		rotation = static_cast<Rotation>(r);
	}
private:
	void wrap_rotation(int& r)
	{
		if (r > 3)
		{
			r = 0;
		}
		else if (r < 0)
		{
			r = 3;
		}
	}
};

class GameMain
{
public:
	GameMain(DirectX11& dx, HINSTANCE hInstance, HWND hWnd, UINT windowSizeW, UINT windowSizeH, std::shared_ptr<DX::IControllerInterface> inController);
	~GameMain();

	void SetGameState(DX::GameState NewState);

	void DoFrame(DirectX11& dx, float deltaTime);
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
	DX::GameState m_GameState;
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
	// State : Input
	// ----------------------
	Keyboard m_keyBoard;
};
