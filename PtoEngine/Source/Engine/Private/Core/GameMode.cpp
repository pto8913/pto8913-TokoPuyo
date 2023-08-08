
#include "Core/GameMode.h"

GameMode::GameMode(NumOfPlayer inNumOfPlayer, NumOfConnect inNumOfConnect)
	: numOfPlayer(inNumOfPlayer), numOfConnect(inNumOfConnect)
{

}

void GameMode::SetNumOfPlayer(NumOfPlayer NewVal) noexcept
{
	numOfPlayer = NewVal;
}
void GameMode::SetNumOfConnect(NumOfConnect NewVal) noexcept
{
	numOfConnect = NewVal;
}
