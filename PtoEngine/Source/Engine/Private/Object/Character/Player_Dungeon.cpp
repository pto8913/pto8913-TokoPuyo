
#include "Object/Character/Player_Dungeon.h"

#include "Level/Level2D.h"

Player_Dungeon::Player_Dungeon(DirectX11& dx)
	: Player(dx)
{

}
Player_Dungeon::~Player_Dungeon()
{

}

// ---------------------------
// Main : Transform
// ---------------------------
void Player_Dungeon::SetActorLocation(const FVector& in)
{
	Player::SetActorLocation(in);
	SetOffset(GetTypedOuter<Level2D>()->WorldToScreen(GetActorLocation().x, GetActorLocation().y, GetActorScale().To2D()));
}

// ---------------------------
// Main : Movement
// ---------------------------
void Player_Dungeon::InputUpdate()
{
	if (InputW)
	{
		Move(0, -1);
	}
	else
	{
		if (InputAxisW && InputAxisLShift)
		{
			Move(0, -1);
		}
		else
		{
			if (InputAxisW.IsPressed())
			{
				bCanMove = true;
			}
		}
	}
	if (InputS)
	{
		Move(0, 1);
	}
	else
	{
		if (InputAxisS && InputAxisLShift)
		{
			Move(0, 1);
		}
		else
		{
			if (InputAxisS.IsPressed())
			{
				bCanMove = true;
			}
		}
	}
	if (InputA)
	{
		Move(-1, 0);
	}
	else
	{
		if (InputAxisA && InputAxisLShift)
		{
			Move(-1, 0);
		}
		else
		{
			if (InputAxisA.IsPressed())
			{
				bCanMove = true;
			}
		}
	}
	if (InputD)
	{
		Move(1, 0);
	}
	else
	{
		if (InputAxisD && InputAxisLShift)
		{
			Move(1, 0);
		}
		else
		{
			if (InputAxisD.IsPressed())
			{
				bCanMove = true;
			}
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
void Player_Dungeon::Move(const int& x, const int& y)
{
	if (bCanMove)
	{
		if (auto pLevel = GetTypedOuter<Level2D>())
		{
			if (pLevel->CanMove(x, y))
			{
				if (pLevel->MoveCenter(x, y))
				{
					SetActorLocation(FVector(pLevel->GetCenter().x, pLevel->GetCenter().y, 0));

					OnPlayerMoved.Broadcast(GetActorLocation());
				}
			}
			else
			{
#if _DEBUG
				OutputDebugStringA("Can not move");
#endif
			}
		}
	}
}

void Player_Dungeon::EnterDungeonBlock(const FVector2D& in)
{
	bCanMove = false;
#if _DEBUG
	OutputDebugStringA("Enter Dungeon Block");
#endif
}