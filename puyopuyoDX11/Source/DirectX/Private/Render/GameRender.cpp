
#include "Render/GameRender.h"


GameRender::GameRender(DirectX11& dx)
{
	m_pGameStateUI = std::make_shared<GameStateUI>(dx);
}