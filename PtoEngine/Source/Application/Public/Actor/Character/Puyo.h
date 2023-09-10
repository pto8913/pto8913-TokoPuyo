#pragma once

#include "Actor/Actor2D.h"
#include "GameSettings.h"

class DirectX11;

struct PuyoSettings;

class Puyo : public Actor2D
{
public:
	Puyo(DirectX11& dx, const uint8_t& type);
	virtual ~Puyo();

	enum class ERotation : uint8_t
	{
		U = 0,
		R = 1,
		B = 2,
		L = 3,
	};
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

	// --------------------------
	// Main : Utils
	// --------------------------
	void SetType(const uint8_t& type);
	uint8_t GetType() const;

	ERotation GetRotation();
	void SetRotation(const ERotation& in);

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
		mRotation = static_cast<ERotation>(r);
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
	// State
	// ------------------------------------------------------
	uint8_t mType;
	ERotation mRotation = ERotation::U;
	bool bIsActive = true;
};