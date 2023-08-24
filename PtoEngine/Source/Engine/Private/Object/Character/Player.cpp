
#include "Object/Character/Player.h"

#include "Level/Level2D.h"
#include "Engine/World.h"

Player::Player(DirectX11& dx)
	: CharacterBase(dx, ECharacterId::Player),
	InputW(DIK_W),
	InputAxisW(DIK_W),
	InputA(DIK_A),
	InputAxisA(DIK_A),
	InputS(DIK_S),
	InputAxisS(DIK_S),
	InputD(DIK_D),
	InputAxisD(DIK_D),
	InputAxisLShift(DIK_LSHIFT),
	InputQ(DIK_Q),
	InputE(DIK_E)
{
	SetTickEnabled(true);
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Player::BeginPlay(DirectX11& dx)
{
	CharacterBase::BeginPlay(dx);
	SetActorLocation(FVector(0, 1, 0));
}

void Player::Tick(DirectX11& dx, float deltaTime)
{
	CharacterBase::Tick(dx, deltaTime);

	if (IsEnableInput())
	{
		InputUpdate();
	}
}

// ---------------------------
// Main : Movement
// ---------------------------
void Player::SetEnableInput(bool in)
{
	bIsEnableInput = in;
}
bool Player::IsEnableInput() const noexcept
{
	return bIsEnableInput;
}
void Player::InputUpdate()
{
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
void Player::Move(const int& x, const int& y)
{
}