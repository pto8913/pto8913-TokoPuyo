
#include "GameMain.h"
#include "GameMode.h"

#include "Window.h"

#include "Object/Sprite.h"

#include "Object/GameStateUI.h"

#include "Math/Math.h"

using namespace DirectX;
using namespace Math;

std::shared_ptr<GameMode> m_pGameMode;

Keyboard::InputAction InputZ(DIK_Z);
Keyboard::InputAction InputX(DIK_X);
Keyboard::InputAction InputLeft(DIK_LEFT);
Keyboard::InputAction InputRight(DIK_RIGHT);
Keyboard::InputAction InputUp(DIK_UP);
Keyboard::InputAxis InputDown(DIK_DOWN);
Keyboard::InputAction InputEsc(DIK_ESCAPE);

UINT8 vanishCount;

GameMain::GameMain(DirectX11& dx, HINSTANCE hInstance, HWND hWnd, UINT windowSizeW, UINT windowSizeH, std::shared_ptr<DX::IControllerInterface> inController)
	: ControllerInterface(inController)
{
	size = (UINT8)Config::GAMESCREEN_WIDTH * Config::GAMESCREEN_HEIGHT;
	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;

	unionFind = UnionFind(size);
	stackedPuyo.resize(size);
	stackedPuyoSprites.resize(size);
	planVanishPuyo.resize(size);
	Cached_NeedDurationTime_Main = 0;

	m_pdx = &dx;

	m_keyBoard = Keyboard(hInstance, hWnd);
	m_pGameStateUI = new GameStateUI(dx, windowSizeW, windowSizeH);m_pGameStateUI->SetGmaeProgressUI(dx);m_pGameStateUI->UpdateScore(0, 0);m_pGameStateUI->OnClickedRestart.Bind<&GameMain::OnClickedRestart>(*this);m_pGameStateUI->OnClickedPause.Bind<&GameMain::OnClickedPause>(*this);m_pGameStateUI->AddToViewport();

	m_pGameMode = std::make_shared<GameMode>();
	vanishCount = static_cast<UINT8>(m_pGameMode->GetNumOfConnect());

	std::random_device rd;
	gen = std::mt19937(rd());
	distr = std::uniform_int_distribution<int>(0, 3);

	BackGround = std::make_shared<Sprite>(
		dx,
		L"Content/Textures/T_Grid_384x768.png",
		L"Background_Grid",
		Config::GAMESCREEN_BACKGROUND_SIZE, 
		Config::GAMESCREEN_PADDING
	);

	LastTime_Main = chrono::now();

	ControllerInterface->GetMouseInterface()->GetClickedLeftPressed().Bind<&GameMain::OnClickedDown>(*this);
	ControllerInterface->GetMouseInterface()->GetClickedLeftReleased().Bind<&GameMain::OnClickedUp>(*this);
	ControllerInterface->GetMouseInterface()->GetClickedLeftHeld().Bind<&GameMain::OnClickedHeld>(*this);

	ControllerInterface->GetMouseInterface()->GetClickedRightPressed().Bind<&GameMain::OnClickedDown>(*this);
	ControllerInterface->GetMouseInterface()->GetClickedRightReleased().Bind<&GameMain::OnClickedUp>(*this);
	ControllerInterface->GetMouseInterface()->GetClickedRightHeld().Bind<&GameMain::OnClickedHeld>(*this);

	ControllerInterface->GetMouseInterface()->GetClickedWheelPressed().Bind<&GameMain::OnClickedDown>(*this);
	ControllerInterface->GetMouseInterface()->GetClickedWheelReleased().Bind<&GameMain::OnClickedUp>(*this);
	ControllerInterface->GetMouseInterface()->GetClickedWheelHeld().Bind<&GameMain::OnClickedHeld>(*this);

	ControllerInterface->GetMouseInterface()->GetMouseMove().Bind<&GameMain::OnMouseMove>(*this);

	StartControlPuyo();
#if _DEBUG
	OutputDebugStringA("DEBUG");
#else
	OutputDebugStringA("NotDebug");
#endif
}
GameMain::~GameMain()
{
}

void GameMain::SetGameState(DX::GameState NewState)
{
	m_GameState = NewState;
	switch (NewState)
	{
	case DX::GameState::Control:
		//OutputDebugStringA("--- GameState Control ---\n");
		NeedDurationTime_Main = Config::PUYO_FALL_SPEED_GAMESTATE_CONTROL;
		break;
	case DX::GameState::Release:
		//OutputDebugStringA("--- GameState Release ---\n");
		NeedDurationTime_Main = Config::PUYO_FALL_SPEED_GAMESTATE_RELEASE;
		DoFrame_Release();
		break;
	case DX::GameState::Vanish:
		//OutputDebugStringA("--- GameState Vanish ---\n");
		StartTime_Vanish = chrono::now();
		NeedDurationTime_Main = Config::PUYO_FALL_SPEED_GAMESTATE_VANISH;
		break;
	case DX::GameState::FallAll:
		//OutputDebugStringA("--- GameState Fallall ---\n");
		NeedDurationTime_Main = Config::PUYO_FALL_SPEED_GAMESTATE_VANISH_TO_FALL;
		break;
	case DX::GameState::GameOver:
		m_pGameStateUI->SetGameOverUI(*m_pdx);
		break;
	default:
		//OutputDebugStringA("--- GameState Wait ---\n");
		NeedDurationTime_Main = Config::PUYO_FALL_SPEED_GAMESTATE_WAIT;
		break;
	}
}

void GameMain::DoFrame(DirectX11& dx, float deltaTime)
{
	dx.BeginFrame();

	if (m_GameState != DX::GameState::GameOver)
	{
		m_pGameStateUI->ExecuteTasks(*m_pdx);

		BackGround->ExecuteTasks(*m_pdx);

		chrono::duration DurationTime_Main = chrono::now() - LastTime_Main;
		Duration_Main = std::chrono::duration_cast<std::chrono::milliseconds>(DurationTime_Main);
		////OutputDebugStringA(std::format("deltaTime {}, Duration {}\n", deltaTime, Duration_Main.count()).c_str());
		if (Duration_Main.count() >= NeedDurationTime_Main)
		{
			LastTime_Main = chrono::now();
			switch (m_GameState)
			{
			case DX::GameState::Control:
				DoFrame_Control();
				break;
			case DX::GameState::Release:
				DoFrame_Release();
				break;
			case DX::GameState::Vanish:
				DoFrame_Vanish();
				break;
			case DX::GameState::FallAll:
				DoFrame_FallAll();
				break;
			case DX::GameState::GameOver:
				break;
			case DX::GameState::GameOverAnimation:
				break;
			default:
				break;
			}
		}

		for (auto sprite : stackedPuyoSprites)
		{
			if (sprite.get())
			{
				sprite.get()->ExecuteTasks(*m_pdx);
			}
		}

		InputUpdate();

		if (m_pActivePuyo.get())
		{
			m_pActivePuyo->ExecuteTasks(*m_pdx);
		}
		if (m_pSubPuyo.get())
		{
			m_pSubPuyo->ExecuteTasks(*m_pdx);
		}
	}
	else
	{
		m_pGameStateUI->ExecuteTasks(*m_pdx);
	}

	HRESULT result = dx.EndFrame();
	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	{
		//OnDeviceLost();
	}
	else
	{
		if (FAILED(result))
		{
			assert(false);
		}
	}
}

void GameMain::StartControlPuyo()
{
	//////OutputDebugStringA("Restart Puyo\n");
	
	int P = GetPos(Config::GAMEOVER_COORD);
	if (IsValidIndex(P))
	{
		if (!stackedPuyo[P].IsEmpty())
		{
			SetGameState(DX::GameState::GameOver);
		}
		else
		{
			SetGameState(DX::GameState::Control);

			ResetCalcScoreCount();
			SpawnPuyo();
		}
	}
	else
	{
		SetGameState(DX::GameState::GameOver);
	}
}

void GameMain::SpawnPuyo()
{
	if (nextPuyo1_1 == Config::EMPTY_PUYO)
	{
		activePuyo.Id = (UINT8)distr(gen);
		activePuyo.x = 2u;
		activePuyo.y = 0u;
		activePuyo.UpdateOffset();

		subId = (UINT8)distr(gen);

		nextPuyo1_1 = (UINT8)distr(gen);
		nextPuyo1_2 = (UINT8)distr(gen);

		nextPuyo2_1 = (UINT8)distr(gen);
		nextPuyo2_2 = (UINT8)distr(gen);
	}
	else
	{
		activePuyo.Id = std::move(nextPuyo1_1);
		activePuyo.x = 2u;
		activePuyo.y = 0u;
		activePuyo.UpdateOffset();

		subId = std::move(nextPuyo1_2);

		nextPuyo1_1 = std::move(nextPuyo2_1);
		nextPuyo1_2 = std::move(nextPuyo2_2);

		nextPuyo2_1 = (UINT8)distr(gen);
		nextPuyo2_2 = (UINT8)distr(gen);
	}

	if (m_pActivePuyo == nullptr)
	{
#if _DEBUG
		OutputDebugStringA("SpawnPuyo new pointer");
#endif
		m_pActivePuyo = std::make_shared<Sprite>(
			*m_pdx,
			Config::PuyoImages[activePuyo.Id].c_str(),
			L"ActivePuyo",
			XMFLOAT2(Config::CELL, Config::CELL),
			activePuyo.offset
		);
		m_pSubPuyo = std::make_shared<Sprite>(
			*m_pdx,
			Config::PuyoImages[subId].c_str(),
			L"SubPuyo",
			XMFLOAT2(Config::CELL, Config::CELL),
			AddXMF2(activePuyo.offset, { 0, -Config::CELL })
		);
	}
	else
	{
#if _DEBUG
		OutputDebugStringA("SpawnPuyo shared pointer");
#endif
		m_pActivePuyo->UpdateTexture(Config::PuyoImages[activePuyo.Id]);
		m_pActivePuyo->SetOffset(activePuyo.offset);
		m_pSubPuyo->UpdateTexture(Config::PuyoImages[subId]);
		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, -Config::CELL }));
	}
	m_pGameStateUI->UpdateNextPuyo(nextPuyo1_1, nextPuyo1_2, nextPuyo2_1, nextPuyo2_2);
}

// ------------------------------------------------------------
// Main : Control Puyo
// ------------------------------------------------------------
void GameMain::DoFrame_Control()
{
	ActionActivePuyoDown(0.5f);
}

// ------------------------------------------------------------
// Main : UI
// ------------------------------------------------------------
void GameMain::OnClickedRestart(DX::MouseEvent)
{
	if (m_GameState == DX::GameState::GameOver)
	{
		m_pGameStateUI->SetGmaeProgressUI(*m_pdx);
	}

	SetGameState(DX::GameState::Wait);

	activePuyo.SetEmpty();
	subPuyo.SetEmpty();
	subId = Config::EMPTY_PUYO;
	m_pActivePuyo.reset();
	m_pSubPuyo.reset();
	nextPuyo1_1 = Config::EMPTY_PUYO;
	nextPuyo1_2 = Config::EMPTY_PUYO;
	nextPuyo2_1 = Config::EMPTY_PUYO;
	nextPuyo2_2 = Config::EMPTY_PUYO;

	unionFind.clear();

	stackedPuyo.erase(stackedPuyo.begin(), stackedPuyo.end());
	stackedPuyo.clear();

	stackedPuyoSprites.erase(stackedPuyoSprites.begin(), stackedPuyoSprites.end());
	stackedPuyoSprites.clear();

	planVanishPuyo.erase(planVanishPuyo.begin(), planVanishPuyo.end());
	planVanishPuyo.clear();

	stackedPuyo.resize(size);
	stackedPuyoSprites.resize(size);
	planVanishPuyo.resize(size);

	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
	score = 0;
	
	LastTime_Main = chrono::now();

	StartControlPuyo();
}
void GameMain::OnClickedPause(DX::MouseEvent)
{
	if (m_pGameStateUI->IsPause())
	{
		Cached_GameState = m_GameState;
		SetGameState(DX::GameState::Wait);
	}
	else
	{
		SetGameState(Cached_GameState);
	}
}

// ---------------------------
// Main : Puyo Moving
// ---------------------------
void GameMain::ActionActivePuyoDown(float rate)
{
	if (m_pActivePuyo == nullptr) return;
	int NAP, NSP;
	bool success = false;
	switch (activePuyo.rotation)
	{
	case Puyo::Rotation::U:
		NAP = GetPos(activePuyo.x, activePuyo.y + 1);
		if (IsValidIndex(NAP))
		{
			if (stackedPuyo[NAP].IsEmpty())
			{
				activePuyo.y += rate;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, -Config::CELL }));
				success = true;
			}
		}
		break;
	case Puyo::Rotation::B:
		NAP = GetPos(activePuyo.x, activePuyo.y + 2);
		if (IsValidIndex(NAP))
		{
			if (stackedPuyo[NAP].IsEmpty())
			{
				activePuyo.y += rate;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, Config::CELL }));
				success = true;
			}
		}
		break;
	case Puyo::Rotation::L:
		NAP = GetPos(activePuyo.x, activePuyo.y + 1);
		NSP = GetPos(activePuyo.x - 1, activePuyo.y + 1);
		if (IsValidIndex(NAP) && IsValidIndex(NSP))
		{
			if (stackedPuyo[NAP].IsEmpty() && stackedPuyo[NSP].IsEmpty())
			{
				activePuyo.y += rate;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { -Config::CELL, 0 }));
				success = true;
			}
		}
		break;
	case Puyo::Rotation::R:
		NAP = GetPos(activePuyo.x, activePuyo.y + 1);
		NSP = GetPos(activePuyo.x + 1, activePuyo.y + 1);
		if (IsValidIndex(NAP) && IsValidIndex(NSP))
		{
			if (stackedPuyo[NAP].IsEmpty() && stackedPuyo[NSP].IsEmpty())
			{
				activePuyo.y += rate;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { Config::CELL, 0 }));
				success = true;
			}
		}
		break;
	default:
		break;
	}
	if (!success)
	{
		ActivePuyoDownToRelease();
	}
}
void GameMain::ActivePuyoDownToRelease()
{
	if (activePuyo.y >= Config::GAMESCREEN_HEIGHT)
	{
		activePuyo.y = floor(activePuyo.y);
	}
	else
	{
		activePuyo.y = ceil(activePuyo.y);
	}
	if (activePuyo.x < 0)
	{
		activePuyo.x = 0;
	}
	else if (activePuyo.x >= Config::GAMESCREEN_WIDTH)
	{
		activePuyo.x = floor(activePuyo.x);
	}
	activePuyo.UpdateOffset();
	m_pActivePuyo->SetOffset(activePuyo.offset);
	activePuyo.CopyToSub(subPuyo);
	subPuyo.Id = subId;
	m_pSubPuyo->SetOffset(subPuyo.offset);
	//OutputDebugStringA(std::format("Release active puyo x:{} y:{} pos:{}\n", activePuyo.x, activePuyo.y, GetPos(activePuyo.x, activePuyo.y)).c_str());
	//OutputDebugStringA(std::format("Release sub puyo x:{} y:{} pos:{}\n", subPuyo.x, subPuyo.y, GetPos(subPuyo.x, subPuyo.y)).c_str());

	activePuyo.UpdateOffset();
	SetGameState(DX::GameState::Release);
}
void GameMain::ActionActivePuyoSlide(bool left)
{
	const float slideDir = left ? -1.f : 1.f;

	if (activePuyo.y == 0)
	{
		ActionActivePuyoDown(0.5f);
	}

	int NAP, NSP;
	bool success = false;
	switch (activePuyo.rotation)
	{
	case Puyo::Rotation::U:
		NAP = GetPos(activePuyo.x + slideDir, activePuyo.y);
		NSP = GetPos(activePuyo.x + slideDir, activePuyo.y - 1);
		if (IsValidIndex(NAP) && IsValidIndex(NSP))
		{
			if (stackedPuyo[NAP].IsEmpty() && stackedPuyo[NSP].IsEmpty())
			{
				activePuyo.x += slideDir;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, -Config::CELL }));
				success = true;
			}
		}
		break;
	case Puyo::Rotation::B:
		NAP = GetPos(activePuyo.x + slideDir, activePuyo.y);
		NSP = GetPos(activePuyo.x + slideDir, activePuyo.y + 1);
		if (IsValidIndex(NAP) && IsValidIndex(NSP))
		{
			if (stackedPuyo[NAP].IsEmpty() && stackedPuyo[NSP].IsEmpty())
			{
				activePuyo.x += slideDir;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, Config::CELL }));
				success = true;
			}
		}
		break;
	case Puyo::Rotation::L:
		if (left)
		{
			NAP = GetPos(activePuyo.x - 2, activePuyo.y);
		}
		else
		{
			NAP = GetPos(activePuyo.x + 1, activePuyo.y);
		}
		if (IsValidIndex(NAP))
		{
			if (stackedPuyo[NAP].IsEmpty())
			{
				activePuyo.x += slideDir;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { -Config::CELL, 0 }));
				success = true;
			}
		}
		break;
	case Puyo::Rotation::R:
		if (left)
		{
			NAP = GetPos(activePuyo.x - 1, activePuyo.y);
		}
		else
		{
			NAP = GetPos(activePuyo.x + 2, activePuyo.y);
		}
		if (IsValidIndex(NAP))
		{
			if (stackedPuyo[NAP].IsEmpty())
			{
				activePuyo.x += slideDir;
				activePuyo.UpdateOffset();
				m_pActivePuyo->SetOffset(activePuyo.offset);
				m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { Config::CELL, 0 }));
				success = true;
			}
		}
		break;
	default:
		break;
	}
}
void GameMain::ActionActivePuyoRotate(bool rotateR)
{
	float x = activePuyo.x, y = activePuyo.y;
	int P;
	/* set rotate coord. */
	if (rotateR)
	{
		switch (activePuyo.rotation)
		{
		case Puyo::Rotation::U:
			/* U to R */
			x += 1;
			P = GetPos(x, y);
			if (!IsValidIndex(P) || !stackedPuyo[P].IsEmpty())
			{
				activePuyo.x -= 1;
				x -= 2;
			}
			break;
		case Puyo::Rotation::B:
			/* B to L */
			x -= 1;
			P = GetPos(x, y);
			if (!IsValidIndex(P) || !stackedPuyo[P].IsEmpty())
			{
				activePuyo.x += 1;
				x += 2;
			}
			break;
		case Puyo::Rotation::L:
			/* L to U */
			y -= 1;
			break;
		case Puyo::Rotation::R:
			/* R to B */
			y += 1;
			P = GetPos(x, y);
			if (!IsValidIndex(P) || !stackedPuyo[P].IsEmpty())
			{
				activePuyo.y -= 1;
				y -= 2;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch (activePuyo.rotation)
		{
		case Puyo::Rotation::U:
			/* U to L */
			x -= 1;
			P = GetPos(x, y);
			if (!IsValidIndex(P) || (IsValidIndex(P) && !stackedPuyo[P].IsEmpty()))
			{
				activePuyo.x += 1;
				x += 2;
			}
			break;
		case Puyo::Rotation::B:
			/* B to R */
			x += 1;
			P = GetPos(x, y);
			if (!IsValidIndex(P) || (IsValidIndex(P) && !stackedPuyo[P].IsEmpty()))
			{
				activePuyo.x -= 1;
				x -= 2;
			}
			break;
		case Puyo::Rotation::L:
			/* L to B */
			y += 1;
			P = GetPos(x, y);
			if (!IsValidIndex(P) || (IsValidIndex(P) && !stackedPuyo[P].IsEmpty()))
			{
				activePuyo.y -= 1;
				y -= 2;
			}
			break;
		case Puyo::Rotation::R:
			/* R to U */
			y -= 1;
			break;
		default:
			break;
		}
	}

	P = GetPos(x, y);
	if (IsValidIndex(P))
	{
		activePuyo.Rotate(rotateR);
		activePuyo.UpdateOffset();
		m_pActivePuyo->SetOffset(activePuyo.offset);
		switch (activePuyo.rotation)
		{
		case Puyo::Rotation::U:
			m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, -Config::CELL }));
			break;
		case Puyo::Rotation::B:
			m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, Config::CELL }));
			break;
		case Puyo::Rotation::L:
			m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { -Config::CELL, 0 }));
			break;
		case Puyo::Rotation::R:
			m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { Config::CELL, 0 }));
			break;
		default:
			break;
		}
		//if (!stackedPuyo[P].IsEmpty())
		//{
		//	y -= 1;
		//	P = GetPos(x, y);
		//	const int NP = GetPos(activePuyo.x, activePuyo.y);
		//	if (IsValidIndex(NP) && IsValidIndex(P))
		//	{
		//		//if (stackedPuyo[NP].IsEmpty()&& stackedPuyo[P].IsEmpty())
		//		//activePuyo.Rotate(rotateR);

		//		//ActivePuyoRotateToRelease();
		//	}
		//}
		//else
		//{
		//	activePuyo.Rotate(rotateR);
		//	m_pActivePuyo->SetOffset(activePuyo.offset);
		//	switch (activePuyo.rotation)
		//	{
		//	case Puyo::Rotation::U:
		//		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, -Config::CELL }));
		//		break;
		//	case Puyo::Rotation::B:
		//		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, Config::CELL }));
		//		break;
		//	case Puyo::Rotation::L:
		//		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { -Config::CELL, 0 }));
		//		break;
		//	case Puyo::Rotation::R:
		//		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { Config::CELL, 0 }));
		//		break;
		//	default:
		//		break;
		//	}
		//}
	}
}
void GameMain::ActivePuyoRotateToRelease()
{
	if (activePuyo.y >= Config::GAMESCREEN_HEIGHT)
	{
		activePuyo.y = floor(activePuyo.y);
	}
	else
	{
		activePuyo.y = ceil(activePuyo.y);
	}
	if (activePuyo.x < 0)
	{
		activePuyo.x = 0;
	}
	else if (activePuyo.x >= Config::GAMESCREEN_WIDTH)
	{
		activePuyo.x = floor(activePuyo.x);
	}
	activePuyo.UpdateOffset();
	m_pActivePuyo->SetOffset(activePuyo.offset);
	activePuyo.CopyToSub(subPuyo);
	subPuyo.Id = subId;
	switch (activePuyo.rotation)
	{
	case Puyo::Rotation::U:
		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, -Config::CELL }));
		break;
	case Puyo::Rotation::B:
		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { 0, Config::CELL }));
		break;
	case Puyo::Rotation::L:
		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { -Config::CELL, -Config::CELL }));
		break;
	case Puyo::Rotation::R:
		m_pSubPuyo->SetOffset(AddXMF2(activePuyo.offset, { Config::CELL, -Config::CELL }));
		break;
	default:
		break;
	}
	//OutputDebugStringA(std::format("Release active puyo x:{} y:{} pos:{}\n", activePuyo.x, activePuyo.y, GetPos(activePuyo.x, activePuyo.y)).c_str());
	//OutputDebugStringA(std::format("Release sub puyo x:{} y:{} pos:{}\n", subPuyo.x, subPuyo.y, GetPos(subPuyo.x, subPuyo.y)).c_str());

	activePuyo.UpdateOffset();
	SetGameState(DX::GameState::Release);
}

// ------------------------------------------------------------
// Main : Release puyo
// ------------------------------------------------------------
void GameMain::DoFrame_Release()
{
	int count = 0;

	if (activePuyo.rotation == Puyo::Rotation::B)
	{
		count += DoFrame_Release(subPuyo, m_pSubPuyo);
		count += DoFrame_Release(activePuyo, m_pActivePuyo);
	}
	else
	{
		count += DoFrame_Release(activePuyo, m_pActivePuyo);
		count += DoFrame_Release(subPuyo, m_pSubPuyo);
	}

	if (count == 0)
	{
		SetGameState(DX::GameState::Wait);
		ReachToBottomActivePuyo();
	}
}
bool GameMain::DoFrame_Release(Puyo& puyo, std::shared_ptr<Sprite> pPuyo)
{
	if (puyo.IsActive)
	{
		int CAP = GetPos(puyo.x, puyo.y);
		int NAP = GetPos(puyo.x, puyo.y + 1);
		if (IsValidIndex(NAP))
		{
			if (stackedPuyo[NAP].IsEmpty())
			{
				//OutputDebugStringA("Stack puyo NAP !!\n");

				puyo.y += 1;
				puyo.UpdateOffset();
				pPuyo->SetOffset(puyo.offset);
				return true;
			}
			else
			{
				if (stackedPuyo[CAP].IsEmpty())
				{
					//OutputDebugStringA("Stack puyo + 1!\n");

					stackedPuyoSprites[CAP] = pPuyo;
					stackedPuyo[CAP] = puyo;
					puyo.IsActive = false;
				}
			}
		}
		else
		{
			if (stackedPuyo[CAP].IsEmpty())
			{
				//OutputDebugStringA(std::format("Stack puyo x:{} y:{} pos:{}\n", puyo.x, puyo.y, CAP).c_str());

				stackedPuyoSprites[CAP] = pPuyo;
				stackedPuyo[CAP] = puyo;
				puyo.IsActive = false;
			}
		}
	}
	return false;
}

void GameMain::ReachToBottomActivePuyo()
{
	//OutputDebugStringA("ReachToBottomActivePuyo\n");
	int AP = GetPos(activePuyo.x, activePuyo.y);
	int SP = GetPos(subPuyo.x, subPuyo.y);

	UnionFindPuyo(activePuyo);
	UnionFindPuyo(subPuyo);

	bool hasVanishPlan = false;
	if (unionFind.size(AP) >= vanishCount)
	{
		hasVanishPlan |= SetPlanToVanishPuyo(activePuyo);
	}
	if (unionFind.size(SP) >= vanishCount)
	{
		hasVanishPlan |= SetPlanToVanishPuyo(subPuyo);
	}

	activePuyo.SetEmpty();
	subPuyo.SetEmpty();

	m_pActivePuyo.reset();
	m_pSubPuyo.reset();

	if (hasVanishPlan)
	{
		SetGameState(DX::GameState::Vanish);
	}
	else
	{
		StartControlPuyo();
	}
}

// ------------------------------------------------------------
// Vanish puyo
// ------------------------------------------------------------
void GameMain::DoFrame_Vanish()
{
	if (m_GameState == DX::GameState::Vanish)
	{
		std::chrono::duration Duration = chrono::now() - StartTime_Vanish;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count() >= Config::PUTO_EFFECT_TIME_VANISH)
		{
			SetVisibilityVanishPuyo(true);
			VanishPuyo();
			SetGameState(DX::GameState::FallAll);
		}
		else
		{
			FlashVanishPuyo();
		}
	}
}
bool GameMain::SetPlanToVanishPuyo(Puyo puyo)
{
	//OutputDebugStringA("SetPlanToVanishPuyo\n");
	const int CP = GetPos(puyo.x, puyo.y);
	int P;
	bool hasVanishPlan = false;
	for (UINT8 y = 0; y < Config::GAMESCREEN_HEIGHT; ++y)
	{
		for (UINT8 x = 0; x < Config::GAMESCREEN_WIDTH; ++x)
		{
			P = GetPos(x, y);
			if (unionFind.isSame(CP, P) && unionFind.size(P) >= vanishCount)
			{
				planVanishPuyo[P] = true;
				hasVanishPlan = true;
			}
		}
	}
	return hasVanishPlan;
}
bool GameMain::SetPlanToVanishAll()
{
	int P;
	bool hasVanishPlan = false;
	for (UINT8 y = 0; y < Config::GAMESCREEN_HEIGHT; ++y)
	{
		for (UINT8 x = 0; x < Config::GAMESCREEN_WIDTH; ++x)
		{
			P = GetPos(x, y);
			if (unionFind.size(P) >= vanishCount)
			{
				planVanishPuyo[P] = true;
				hasVanishPlan = true;
			}
		}
	}
	return hasVanishPlan;
}
void GameMain::ResetPlanToVanishPuyo()
{
	std::fill(planVanishPuyo.begin(), planVanishPuyo.end(), false);
}
void GameMain::FlashVanishPuyo()
{
	VanishPuyoVisibility = !VanishPuyoVisibility;
	SetVisibilityVanishPuyo(VanishPuyoVisibility);
}
void GameMain::SetVisibilityVanishPuyo(bool in)
{
	int P;
	for (UINT8 y = 0; y < Config::GAMESCREEN_HEIGHT; ++y)
	{
		for (UINT8 x = 0; x < Config::GAMESCREEN_WIDTH; ++x)
		{
			P = GetPos(x, y);
			if (planVanishPuyo[P])
			{
				if (stackedPuyoSprites[P] != nullptr)
				{
					stackedPuyoSprites[P]->SetVisibility(in);
				}
			}
		}
	}
}
void GameMain::VanishPuyo()
{
	//OutputDebugStringA("Vanish Puyo\n");

	int P;
	std::map<int, int> colors;
	std::map<int, int> RootSize;
	for (UINT8 y = 0; y < Config::GAMESCREEN_HEIGHT; ++y)
	{
		for (UINT8 x = 0; x < Config::GAMESCREEN_WIDTH; ++x)
		{
			P = GetPos(x, y);
			if (planVanishPuyo[P])
			{
				Puyo& puyo = stackedPuyo[P];

				/* Cache for calculate score. */
				++colors[puyo.Id];
				RootSize.emplace(std::make_pair<int, int>(unionFind.root(P), unionFind.size(P)));

				/* Clear puyo. */
				puyo.SetEmpty();
				stackedPuyoSprites[P].reset();
				planVanishPuyo[P] = false;
			}
		}
	}

	/* calc score */
	int bonus = 0;
	for (auto&& s : RootSize)
	{
		bonus += Config::connectBonus[s.second];
		puyoCount += s.second;
	}
	for (auto&& c : colors)
	{
		if (c.second > 0)
		{
			++colorCount;
		}
	}
	bonus += Config::colorBonus[colorCount];
	++comboCount;
	bonus += Config::comboBonus[comboCount];
	bonus = bonus == 0 ? 1 : bonus;
	
	score += puyoCount * 10 * bonus;

	m_pGameStateUI->UpdateScore(score, comboCount);

	colorCount = 0;
	puyoCount = 0;
}

// ------------------------------------------------------------
// Fall puyo
// ------------------------------------------------------------
void GameMain::DoFrame_FallAll()
{
	bool bIsDown = false;
	int CP, NP;
	for (int y = Config::GAMESCREEN_HEIGHT - 1; y >= 0; --y)
	{
		for (int x = Config::GAMESCREEN_WIDTH - 1; x >= 0; --x)
		{
			CP = GetPos((float)x, (float)y);
			if (IsValidIndex(CP))
			{
				Puyo& curPuyo = stackedPuyo[CP];
				if (!curPuyo.IsEmpty())
				{
					auto& pcurPuyo = stackedPuyoSprites[CP];

					NP = GetPos(curPuyo.x, curPuyo.y + 1);
					if (IsValidIndex(NP))
					{
						if (stackedPuyo[NP].IsEmpty())
						{
							curPuyo.y += 1;
							curPuyo.UpdateOffset();
							pcurPuyo->SetOffset(curPuyo.offset);
							bIsDown = true;
							stackedPuyo[NP] = curPuyo;
							curPuyo.SetEmpty();
							stackedPuyoSprites[NP] = std::move(pcurPuyo);
							stackedPuyoSprites[CP].reset();
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
void GameMain::ReachToBottomAll()
{
	ResetPlanToVanishPuyo();
	RemakeUnionFind();

	if (SetPlanToVanishAll())
	{
		SetGameState(DX::GameState::Vanish);
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
void GameMain::RemakeUnionFind()
{
	unionFind.clear();

	int P;
	for (UINT8 y = 0; y < Config::GAMESCREEN_HEIGHT; ++y)
	{
		for (UINT8 x = 0; x < Config::GAMESCREEN_WIDTH; ++x)
		{
			P = GetPos(x, y);
			if (stackedPuyo[P].IsEmpty())
			{
				continue;
			}
			UnionFindPuyo(stackedPuyo[P]);
		}
	}
}
void GameMain::UnionFindPuyo(Puyo puyo)
{
	float x = puyo.x, y = puyo.y;
	const int CP = GetPos(x, y);
	//OutputDebugStringA(std::format("CP {} = {} * {} + {}\n", CP, ceil(y), Config::GAMESCREEN_WIDTH, ceil(x)).c_str());
	auto Check = [this, &puyo, &CP](int P, LPCSTR str)
	{
		if (IsValidIndex(P))
		{
			if (puyo.IsSameId(stackedPuyo[P]))
			{
				//OutputDebugStringA(std::format("puyo id : {}, id2 : {}\n", puyo.Id, stackedPuyo[P].Id).c_str());
				//OutputDebugStringA(str);
				unionFind.unite(CP, P);
			}
		}
	};
	//OutputDebugStringA(std::format("{} = {} * {} + {}\n", GetPos(x - 1, y), ceil(y), Config::GAMESCREEN_WIDTH, ceil(x - 1)).c_str());
	Check(GetPos(x - 1, y), "Unite Left\n");

	//OutputDebugStringA(std::format("{} = {} * {} + {}\n", GetPos(x + 1, y), ceil(y), Config::GAMESCREEN_WIDTH, ceil(x + 1)).c_str());
	Check(GetPos(x + 1, y), "Unite Right\n");

	//OutputDebugStringA(std::format("{} = {} * {} + {}\n", GetPos(x, y - 1), ceil(y - 1), Config::GAMESCREEN_WIDTH, ceil(x)).c_str());
	Check(GetPos(x, y - 1), "Unite Up\n");

	//OutputDebugStringA(std::format("{} = {} * {} + {}\n", GetPos(x, y + 1), ceil(y + 1), Config::GAMESCREEN_WIDTH, ceil(x)).c_str());
	Check(GetPos(x, y + 1), "Unite Bottom\n");
}

// ------------------------------------------------------------
// Main : Input
// ------------------------------------------------------------
void GameMain::InputUpdate()
{
	m_keyBoard.Update();

	if (InputEsc)
	{
		PostQuitMessage(0);
	}

	if (m_GameState == DX::GameState::Control)
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
			ActionActivePuyoSlide(true);
		}
		if (InputRight)
		{
			ActionActivePuyoSlide(false);
		}
		if (InputUp)
		{
			ActionActivePuyoRotate();
		}
		if (InputDown)
		{
			SetNeedDurationCached(Config::PUYO_FALL_SPEED_GAMESTATE_PRESS_DOWN);
		}
		else
		{
			ResetNeedDuration();
		}
	}
}
void GameMain::OnClickedDown(DX::MouseEvent inMouseEvent)
{
	m_pGameStateUI->OnMouseButtonDown(inMouseEvent);
}
void GameMain::OnClickedHeld(DX::MouseEvent inMouseEvent)
{
	m_pGameStateUI->OnMouseButtonHeld(inMouseEvent);
}
void GameMain::OnClickedUp(DX::MouseEvent inMouseEvent)
{
	m_pGameStateUI->OnMouseButtonUp(inMouseEvent);
}
void GameMain::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	m_pGameStateUI->OnMouseMove(inMouseEvent);
}

// ------------------------------------------------------
// Main : State : Main Timer
// ------------------------------------------------------
void GameMain::SetNeedDurationCached(DWORD NewVal)
{
	if (m_GameState == DX::GameState::Control)
	{
		if (Cached_NeedDurationTime_Main == 0)
		{
			Cached_NeedDurationTime_Main = NeedDurationTime_Main;
			NeedDurationTime_Main = NewVal;
		}
	}
}
void GameMain::ResetNeedDuration()
{
	if (m_GameState == DX::GameState::Control)
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
void GameMain::ResetCalcScoreCount()
{
	puyoCount = 0;
	comboCount = 0;
	connectCount = 0;
	colorCount = 0;
}

// ------------------------------------------------------------
// Main : Utils
// ------------------------------------------------------------
int GameMain::GetPos(UINT8 x, UINT8 y)
{
	// 33 = 5 * 6 + 3
	////OutputDebugStringA(std::format("{} = {} * {} + {}\n", y * Config::GAMESCREEN_WIDTH+x, y, Config::GAMESCREEN_WIDTH, x).c_str());
	if (x < 0 || x >= Config::GAMESCREEN_WIDTH || y < 0 || y >= Config::GAMESCREEN_HEIGHT)
	{
		return Config::INDEX_NONE;
	}
	return y * Config::GAMESCREEN_WIDTH + x;
}
int GameMain::GetPos(DirectX::XMFLOAT2 in)
{
	return GetPos(in.x, in.y);
}
int GameMain::GetPos(float x, float y)
{
	if (x < 0 || x >= Config::GAMESCREEN_WIDTH || y < -1 || y >= Config::GAMESCREEN_HEIGHT)
	{
		return Config::INDEX_NONE;
	}
	return GetPos((UINT8)ceil(x), (UINT8)ceil(y));
}
void GameMain::GetXYFromPos(UINT8 pos, UINT8& x, UINT8& y)
{
	// x = 33 % 6 = 3;
	x = (UINT8)(pos % Config::GAMESCREEN_WIDTH);
	// y = 33 / 6 = 5;
	y = (UINT8)(pos / Config::GAMESCREEN_WIDTH);
}
