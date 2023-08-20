
#include "Object/Character/Player.h"

#include "Input/Keyboard.h"

#include "Level/Level.h"

Keyboard::InputAction InputW(DIK_W);
Keyboard::InputAxis InputAxisW(DIK_W);
Keyboard::InputAction InputA(DIK_A);
Keyboard::InputAxis InputAxisA(DIK_A);
Keyboard::InputAction InputS(DIK_S);
Keyboard::InputAxis InputAxisS(DIK_S);
Keyboard::InputAction InputD(DIK_D);
Keyboard::InputAxis InputAxisD(DIK_D);
Keyboard::InputAxis InputAxisLShift(DIK_LSHIFT);
Keyboard::InputAction InputQ(DIK_Q);
Keyboard::InputAction InputE(DIK_E);

Player::Player(DirectX11& dx)
	: CharacterBase(dx, ECharacterId::Player)
{
	SetTickEnabled(true);
}

void Player::Tick(DirectX11& dx, float deltaTime)
{
	CharacterBase::Tick(dx, deltaTime);

	InputUpdate();
}

void Player::InputUpdate()
{
	if (InputW)
	{
		GetTypedOuter<Level2D>()->MoveCenter(0, -1);
	}
	else
	{
		if (InputAxisW && InputAxisLShift)
		{
			GetTypedOuter<Level2D>()->MoveCenter(0, -1);
		}
	}
	if (InputS)
	{
		GetTypedOuter<Level2D>()->MoveCenter(0, 1);
	}
	else
	{
		if (InputAxisS && InputAxisLShift)
		{
			GetTypedOuter<Level2D>()->MoveCenter(0, 1);
		}
	}
	if (InputA)
	{
		GetTypedOuter<Level2D>()->MoveCenter(-1, 0);
	}
	else
	{
		if (InputAxisA && InputAxisLShift)
		{
			GetTypedOuter<Level2D>()->MoveCenter(-1, 0);
		}
	}
	if (InputD)
	{
		GetTypedOuter<Level2D>()->MoveCenter(1, 0);
	}
	else
	{
		if (InputAxisD && InputAxisLShift)
		{
			GetTypedOuter<Level2D>()->MoveCenter(1, 0);
		}
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