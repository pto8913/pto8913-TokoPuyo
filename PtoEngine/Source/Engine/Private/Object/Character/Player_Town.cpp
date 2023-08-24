
#include "Object/Character/Player_Town.h"

#include "Component/MovementComponent.h"

#include "Level/Level2D.h"

Player_Town::Player_Town(DirectX11& dx)
	: Player(dx)
{

}
Player_Town::~Player_Town()
{

}

// ---------------------------
// Main : Transform
// ---------------------------
void Player_Town::SetActorLocation(const FVector& in)
{
	Player::SetActorLocation(in);
	SetOffset(GetTypedOuter<Level2D>()->WorldToScreen(GetActorLocation().x, GetActorLocation().y, GetActorScale().To2D()));
}

// ---------------------------
// Main : Movement
// ---------------------------
void Player_Town::InputUpdate()
{
	if (InputW)
	{
		Move(0, -1);
	}
	else
	{
		if (InputAxisW.IsPressed())
		{
			GetMovementComp()->StartInput();
		}
		if (InputAxisW)
		{
			Move(0, -1);
		}
		else
		{
			GetMovementComp()->EndInput();
		}
	}
	if (InputS)
	{
		Move(0, 1);
	}
	else
	{
		if (InputAxisS.IsPressed())
		{
			GetMovementComp()->StartInput();
		}
		if (InputAxisS)
		{
			Move(0, 1);
		}
		else
		{
			GetMovementComp()->EndInput();
		}
	}
	if (InputA)
	{
		Move(-1, 0);
	}
	else
	{
		if (InputAxisA.IsPressed())
		{
			GetMovementComp()->StartInput();
		}
		if (InputAxisA)
		{
			Move(-1, 0);
		}
		else
		{
			GetMovementComp()->EndInput();
		}
	}
	if (InputD)
	{
		Move(1, 0);
	}
	else
	{
		if (InputAxisD.IsPressed())
		{
			GetMovementComp()->StartInput();
		}
		if (InputAxisD)
		{
			Move(1, 0);
		}
		else
		{
			GetMovementComp()->EndInput();
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
void Player_Town::Move(const int& x, const int& y)
{
	if (bCanMove)
	{
		GetMovementComp()->AddVelocity(FVector(x, y, 0));
		//GetTypedOuter<Level2D>()->MoveCenter(x, y);

		OnPlayerMoved.Broadcast(GetActorLocation());
	}
}