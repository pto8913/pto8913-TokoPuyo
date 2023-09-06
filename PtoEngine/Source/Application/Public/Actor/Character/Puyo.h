#pragma once

#include "Actor/Character/CharacterBase.h"

struct Puyo
{
public:
	Puyo() : Id(Config::EMPTY_PUYO), IsActive(true), rotation(Rotation::U) {}
	Puyo(UINT8 inId) : Id(inId), IsActive(true), rotation(Rotation::U) {}

	void CopyToSub(Puyo& target)
	{
		target.rotation = rotation;
		target.x = x;
		target.y = y;
		switch (rotation)
		{
		case Rotation::U:
			target.y = y - 1;
			break;
		case Rotation::R:
			target.x = x + 1;
			break;
		case Rotation::B:
			target.y = y + 1;
			break;
		case Rotation::L:
			target.x = x - 1;
			break;
		default:
			break;
		}
		target.UpdateOffset();
	}

	bool IsSame(const Puyo& In) const
	{
		return Id == In.Id && x == In.x && y == In.y;
	}
	bool IsSameId(const Puyo& In) const
	{
		return Id == In.Id;
	}
	bool IsEmpty() const
	{
		return Id == Config::EMPTY_PUYO;
	}

	void SetEmpty()
	{
		Id = Config::EMPTY_PUYO;
		IsActive = true;
		rotation = Rotation::U;
		offset = { 0, 0 };
		x, y = 0;
	}

	void UpdateOffset(DirectX::XMFLOAT2 Additional = { 0, 0 })
	{
		offset.x = Config::GAMESCREEN_PADDING.x + x * Config::CELL + Additional.x;
		offset.y = Config::GAMESCREEN_PADDING.y + (y - 1) * Config::CELL + Additional.y;
	}

	/* 0 ~ 3 */
	UINT8 Id;
	bool IsActive;

	/* x : 0 ~W - 1 */
	float x;
	/* y: 0 ~H - 1 */
	float y;

	DirectX::XMFLOAT2 offset;

	enum class Rotation : UINT8
	{
		U = 0,
		R = 1,
		B = 2,
		L = 3,
	};

	Rotation rotation;
	void Rotate(bool rotateR)
	{
		int r = static_cast<int>(rotation);
		if (rotateR)
		{
			wrap_rotation(++r);
		}
		else
		{
			wrap_rotation(--r);
		}
		rotation = static_cast<Rotation>(r);
	}
private:
	void wrap_rotation(int& r)
	{
		if (r > 3)
		{
			r = 0;
		}
		else if (r < 0)
		{
			r = 3;
		}
	}
};


class Enemy : public CharacterBase
{
public:
	Enemy(DirectX11& dx, const ECharacterId& CharacterType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

	virtual void TurnElapsed() override {};

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
};