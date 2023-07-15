#pragma once

class DirectX11;

namespace DX
{
	enum class GameState
	{
		Control, /* Control Puyo*/
		Release, /* Release Control Puyo. puyo is fall down to bottom. */
		Vanish,  /* Vanish Puyo */
		FallAll, /* Fall all puyo, from vanish. */
		Wait,    /* Wait Task */
		GameOver,
	};

	enum class GameInfoOverlayCommand {
		None,

	};

	/* Interface */
	struct IGameStateUIControl
	{
		virtual void ExecuteTasks2(DirectX11& dx) = 0;
		virtual void SetPause() = 0;

		virtual void SetGmaeProgressUI(DirectX11& dx) = 0;
		virtual void SetGameOverUI(DirectX11& dx) = 0;
	};
}