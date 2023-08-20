#pragma once

#include "Rotator.h"
#include "Engine/Vector.h"

struct FMatrix
{
public:
	FMatrix() = default;

	FMatrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	) noexcept;
	FMatrix(FQuaternion In);
	FMatrix(FVector In);

	FMatrix operator*(const FMatrix& In);


	float m[4][4];
};

namespace Matrix
{
	FMatrix LookAtLH(FVector EyePosition, FVector FocusPosition, FVector UpDirection);
	FMatrix Transpose(FMatrix In);
};