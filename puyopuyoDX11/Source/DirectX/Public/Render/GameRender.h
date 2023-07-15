#pragma once

#include "DirectX/DirectXHead.h"
#include "GameUIController.h"
#include "Object/GameStateUI.h"

class DirectX11;

class GameRender
{
public:
	GameRender(DirectX11& dx);

	DX::IGameStateUIControl* GetGameStateUI() { return m_pGameStateUI.get(); }
private:
	std::shared_ptr<GameStateUI> m_pGameStateUI;
};