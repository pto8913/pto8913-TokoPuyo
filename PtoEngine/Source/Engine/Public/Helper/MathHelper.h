#pragma once

#include "Engine/Vector.h"
#include "Engine/Box.h"

namespace Vector
{
	FVector Normalize(FVector In);
	float DotProduct(const FVector& a, const FVector& b);
	FVector CrossProduct(const FVector& a, const FVector& b);
	FVector RotateVector(FVector Target, float yaw, float pitch, float roll);
	FVector Clamp(FVector v, float inMin, float inMax);
};

namespace Box
{
	bool IsInBox(const FBox& a, const FBox& b);
	bool IsPointInBox(const FBox& a, const FVector& b);
}