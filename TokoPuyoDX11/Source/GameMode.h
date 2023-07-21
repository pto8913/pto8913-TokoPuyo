#pragma once

#include "DirectX/DirectXHead.h"

class GameMode
{
public:
	enum class NumOfPlayer : UINT8
	{
		P1 = 1,
		//P2 = 2,
		//P4 = 4,
	};
	enum class NumOfConnect : UINT8
	{
		C4 = 4,
		C3 = 3,
	};

	GameMode(NumOfPlayer numPlayer = NumOfPlayer::P1, NumOfConnect numConnect = NumOfConnect::C4);

	NumOfPlayer GetNumOfPlayer() const noexcept { return numOfPlayer; }
	NumOfConnect GetNumOfConnect() const noexcept { return numOfConnect; }

	void SetNumOfPlayer(NumOfPlayer NewVal) noexcept;
	void SetNumOfConnect(NumOfConnect NewVal) noexcept;

protected:
	NumOfPlayer numOfPlayer;
	NumOfConnect numOfConnect;
};