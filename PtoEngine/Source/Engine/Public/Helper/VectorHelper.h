#pragma once

#include "Math/Vector.h"
#include <DirectXMath.h>

namespace Vector
{
	FVector Normalize(FVector In);
	float DotProduct(const FVector& a, const FVector& b);
	FVector CrossProduct(const FVector& a, const FVector& b);
	FVector RotateVector(FVector Target, float yaw, float pitch, float roll);
	FVector Clamp(FVector v, float inMin, float inMax);

	DirectX::XMFLOAT2 ConvertVector2DToDX(const FVector2D& inVector);
	FVector2D ConvertDXToVector2D(const DirectX::XMFLOAT2& inVector);
};