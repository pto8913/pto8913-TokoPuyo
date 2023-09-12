#pragma once

#include <stdint.h>

enum class EPuyoControl
{
	Wait,
	Control,
	Release,
	Vanish,
	VanishToFall,
	PressDown
};

struct FPuyoInfos
{
	friend class Puyo;
public:
	enum class ERotation : uint8_t
	{
		U = 0,
		R = 1,
		B = 2,
		L = 3,
	};
	FPuyoInfos();
	operator bool() const noexcept;
	
	uint8_t GetType() const;
	void SetType(const uint8_t& type);

	ERotation GetRotation() const;
	void SetRotation(const ERotation& in);

	bool GetIsActive() const;
	void SetIsActive(const bool& in);

	bool IsSameType(const FPuyoInfos& in) const;
	bool IsValid() const;

	int GetID() const;
private:
	void SetID(int inID);
public:
	int GetBottomPuyoID() const;
	void MergeBottom(int bottomPuyoID);
	void ClearBottom();

	void Rotate(bool rotateR);
private:
	void wrap_rotation(int& r);
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	uint8_t mType = 5;
	ERotation mRotation = ERotation::U;
	bool bIsActive = true;

	int mParentID = 0;
	int mBottomPuyoID = -1;
};