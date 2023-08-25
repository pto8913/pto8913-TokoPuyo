
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

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Player_Town::BeginPlay(DirectX11& dx)
{
	Player::BeginPlay(dx);

	SetActorLocation(FVector(0,0,0));
	SetOffset(GetTypedOuter<Level2D>()->WorldToScreen(0, 0, GetActorScale().To2D()));
}
void Player_Town::Tick(DirectX11& dx, float deltaTime)
{
	Player::Tick(dx, deltaTime);

	SetOffset(GetTypedOuter<Level2D>()->WorldToScreen(GetActorLocation().x, GetActorLocation().y, GetActorScale().To2D()));
}

// ---------------------------
// Main : Movement
// ---------------------------
void Player_Town::InputUpdate()
{
	if (InputAxisW)
	{
		Move(0, 1);
	}
	if (InputAxisS)
	{
		Move(0, -1);
	}
	if (InputAxisA)
	{
		Move(-1, 0);
	}
	if (InputAxisD)
	{
		Move(1, 0);
	}
	if (!InputAxisW && !InputAxisS && !InputAxisA && !InputAxisD)
	{
		if (GetMovementComp()->IsStarted())
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
		if (!GetMovementComp()->IsStarted())
		{
			GetMovementComp()->StartInput();
			return;
		}

		GetMovementComp()->AddVelocity(FVector(x, 0, y));

		OnPlayerMoved.Broadcast(GetActorLocation());
	}
}