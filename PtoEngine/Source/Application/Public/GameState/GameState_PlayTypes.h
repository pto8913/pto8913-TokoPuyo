#pragma once

enum class EGameProgress
{
	Control, /* Control Puyo*/
	Release, /* Release Control Puyo. puyo is fall down to bottom. */
	Vanish,  /* Vanish Puyo */
	FallAll, /* Fall all puyo, from vanish. */
	Wait,    /* Wait Task */
	GameOver,
	GameOverAnimation,
};
