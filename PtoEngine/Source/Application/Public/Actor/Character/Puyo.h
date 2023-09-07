#pragma once

#include "Actor/Actor2D.h"
#include "GameSettings.h"

class DirectX11;
struct PuyoSettings;

class Puyo : public Actor2D
{
public:
	Puyo(DirectX11& dx, const uint8_t& type);

	enum class Rotation : UINT8
	{
		U = 0,
		R = 1,
		B = 2,
		L = 3,
	};
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetType(const uint8_t& type);
	uint8_t GetType() const;

	Rotation GetRotation();
	void SetRotation(const Rotation& in);

	bool GetIsActive();
	void SetIsActive(const bool& in);

	bool IsSame(const std::shared_ptr<Puyo>& in);
	bool IsSameType(const std::shared_ptr<Puyo>& in);

	void Rotate(bool rotateR)
	{
		int r = static_cast<int>(mRotation);
		if (rotateR)
		{
			wrap_rotation(++r);
		}
		else
		{
			wrap_rotation(--r);
		}
		mRotation = static_cast<Rotation>(r);
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
private:
	// ------------------------------------------------------
	// Sub
	// ------------------------------------------------------
	uint8_t mType;
	Rotation mRotation;
	bool bIsActive = true;
};

struct PuyoS
{
public:
	PuyoS() : Id(GameSettings::EMPTY_PUYO), IsActive(true), rotation(Rotation::U) {}
	PuyoS(UINT8 inId) : Id(inId), IsActive(true), rotation(Rotation::U) {}

	void CopyToSub(PuyoS& target)
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

	bool IsSame(const PuyoS& In) const
	{
		return Id == In.Id && x == In.x && y == In.y;
	}
	bool IsSameId(const PuyoS& In) const
	{
		return Id == In.Id;
	}
	bool IsEmpty() const
	{
		return Id == GameSettings::EMPTY_PUYO;
	}

	void SetEmpty()
	{
		Id = GameSettings::EMPTY_PUYO;
		IsActive = true;
		rotation = Rotation::U;
		offset = { 0, 0 };
		x, y = 0;
	}

	void UpdateOffset(DirectX::XMFLOAT2 Additional = { 0, 0 })
	{
		offset.x = GameSettings::GAMESCREEN_PADDING.x + x * GameSettings::CELL + Additional.x;
		offset.y = GameSettings::GAMESCREEN_PADDING.y + (y - 1) * GameSettings::CELL + Additional.y;
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

