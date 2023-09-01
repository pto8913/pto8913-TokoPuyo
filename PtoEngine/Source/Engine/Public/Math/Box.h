#pragma once

#include "Vector.h"

// ------------------------------------------------------
// FBox
// ------------------------------------------------------
struct FBox
{
public:
	FBox(const FVector& left, const FVector& right);
	FBox(FVector&& left, FVector&& right);

	FVector leftTop;
	FVector rightBottom;

	float GetWidth() const;
	float GetHeight() const;
};