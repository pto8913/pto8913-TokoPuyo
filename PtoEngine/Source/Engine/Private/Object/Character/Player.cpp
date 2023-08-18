
#include "Object/Character/Player.h"

#include "Engine/World.h"

#include "Input/Keyboard.h"

#include "GameInstance.h"

Keyboard::InputAction InputW(DIK_W);
Keyboard::InputAction InputA(DIK_A);
Keyboard::InputAction InputS(DIK_S);
Keyboard::InputAction InputD(DIK_D);
Keyboard::InputAction InputQ(DIK_Q);
Keyboard::InputAction InputE(DIK_E);

Player::Player(DirectX11& dx)
	: CharacterBase(dx, ECharacterId::Player)
{
	SetTickEnabled(true);
}

void Player::Tick(DirectX11& dx, float deltaTime)
{
	InputUpdate();
}

void Player::InputUpdate()
{
	GameInstance& gameInstance = GameInstance::Get();
	if (InputW)
	{
		gameInstance.GetWorld()->MoveCenter(0, -1);
	}
	if (InputS)
	{
		gameInstance.GetWorld()->MoveCenter(0, 1);
	}
	if (InputA)
	{
		gameInstance.GetWorld()->MoveCenter(-1, 0);
	}
	if (InputD)
	{
		gameInstance.GetWorld()->MoveCenter(1, 0);
	}
	if (InputQ)
	{
#if _DEBUG
		OutputDebugStringA("Q\n");
#endif
	}
	if (InputE)
	{
#if _DEBUG
		OutputDebugStringA("E\n");
#endif
	}
}