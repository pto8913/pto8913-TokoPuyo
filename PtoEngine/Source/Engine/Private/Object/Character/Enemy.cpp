
#include "Object/Character/Enemy.h"

Enemy::Enemy(DirectX11& dx, const ECharacterId& CharacterType, const UINT16& inX, const UINT16& inY)
	: CharacterBase(dx, CharacterType),
	x(inX),
	y(inY)
{
}

DirectX::XMFLOAT2 Enemy::GetCoord()
{
	return DirectX::XMFLOAT2(x, y);
}