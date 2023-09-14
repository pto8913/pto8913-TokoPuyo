
#include "Actor/Character/PuyoTypes.h"
#include "GameSettings.h"

// ------------------------------------------------------
// Main
// ------------------------------------------------------
FPuyoInfos::FPuyoInfos(int inID)
	: mParentID(inID)
{
	mType = GameSettings::EMPTY_PUYO;
	mRotation = ERotation::U;
	bIsActive = true;
}
FPuyoInfos::operator bool() const noexcept
{
	return IsValid();
}

uint8_t FPuyoInfos::GetType() const
{
	return mType;
}
void FPuyoInfos::SetType(const uint8_t& type)
{
	mType = type;
}

FPuyoInfos::ERotation FPuyoInfos::GetRotation() const
{
	return mRotation;
}
void FPuyoInfos::SetRotation(const FPuyoInfos::ERotation& in)
{
	mRotation = in;
}

bool FPuyoInfos::GetIsActive() const
{
	return bIsActive;
}
void FPuyoInfos::SetIsActive(const bool& in)
{
	bIsActive = in;
}

bool FPuyoInfos::IsSameType(const FPuyoInfos& in) const
{
	return GetType() == in.GetType();
}
bool FPuyoInfos::IsValid() const
{
	return mType != GameSettings::EMPTY_PUYO;
}

int FPuyoInfos::GetID() const
{
	return mParentID;
}
void FPuyoInfos::SetID(int inID)
{
	mParentID = inID;
}

void FPuyoInfos::Rotate(bool rotateR)
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
void FPuyoInfos::wrap_rotation(int& r)
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