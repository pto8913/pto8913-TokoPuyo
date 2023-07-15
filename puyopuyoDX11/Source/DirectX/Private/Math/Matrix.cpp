
#include "Math/Matrix.h"
#include "Math/Rotator.h"

FMatrix::FMatrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
) noexcept
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}
FMatrix::FMatrix(FQuaternion In)
{
	m[0][0] = 2 * (In.x * In.x + In.y * In.y) - 1;
	m[0][1] = 2 * (In.y * In.z - In.x * In.w);
	m[0][2] = 2 * (In.y * In.w + In.x * In.z);
	m[0][3] = 0;

	m[1][0] = 2 * (In.y * In.z + In.x * In.w);
	m[1][1] = 2 * (In.x * In.x + In.z * In.z) - 1;
	m[1][2] = 2 * (In.z * In.w - In.x * In.y);
	m[1][3] = 0;

	m[2][0] = 2 * (In.y * In.w - In.x * In.z);
	m[2][1] = 2 * (In.z * In.w + In.x * In.y);
	m[2][2] = 2 * (In.x * In.x + In.w * In.w) - 1;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}
FMatrix::FMatrix(FVector In)
{
	m[0][0] = 1;    m[0][1] = 0;    m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = 1;    m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;    m[2][3] = 0;
	m[3][0] = In.x; m[3][1] = In.y; m[3][2] = In.z; m[3][3] = 1;
}

FMatrix FMatrix::operator*(const FMatrix& In)
{
	FMatrix M;

	auto Set = [this, &M, In](int col)
	{
		float x = m[col][0];
		float y = m[col][1];
		float z = m[col][2];
		float w = m[col][3];
		M.m[col][0] = In.m[0][0] * x + In.m[1][0] * y + In.m[2][0] * z + In.m[3][0] * w;
		M.m[col][1] = In.m[0][1] * x + In.m[1][1] * y + In.m[2][1] * z + In.m[3][1] * w;
		M.m[col][2] = In.m[0][2] * x + In.m[1][2] * y + In.m[2][2] * z + In.m[3][2] * w;
		M.m[col][3] = In.m[0][3] * x + In.m[1][3] * y + In.m[2][3] * z + In.m[3][3] * w;
	};
	Set(0);
	Set(1);
	Set(2);
	Set(3);
	return M;
}

namespace Matrix
{
	FMatrix LookAtLH(FVector EyePosition, FVector FocusPosition, FVector UpDirection)
	{
		FVector EyeDirection = FocusPosition - EyePosition;
		/* Forward Vector */
		FVector FV = EyeDirection.Normalize();
		/* Right Vector */
		FVector RV = Vector::CrossProduct(UpDirection, FV).Normalize();
		/* Up Vector */
		FVector UV = Vector::CrossProduct(FV, RV);

		FVector InvEyeDirection = EyeDirection * -1.f;

		float Dot0 = Vector::DotProduct(RV, InvEyeDirection);
		float Dot1 = Vector::DotProduct(UV, InvEyeDirection);
		float Dot2 = Vector::DotProduct(FV, InvEyeDirection);

		FMatrix M;

		M.m[0][0] = RV.x; M.m[0][1] = RV.y; M.m[0][2] = RV.z; M.m[0][3] = Dot0;
		M.m[1][0] = UV.x; M.m[1][1] = UV.y; M.m[1][2] = UV.z; M.m[1][3] = Dot1;
		M.m[2][0] = FV.z; M.m[2][1] = FV.z; M.m[2][2] = FV.z; M.m[2][3] = Dot2;
		M.m[3][0] = 0.f;  M.m[3][1] = 0.f;  M.m[3][2] = 0.f;  M.m[3][3] = 1.f;

		return Transpose(M);
	}

	FMatrix Transpose(FMatrix M)
	{
		FMatrix MT;
		MT.m[0][0] = M.m[0][0]; MT.m[0][1] = M.m[1][0]; MT.m[0][2] = M.m[2][0]; MT.m[0][3] = M.m[3][0];
		MT.m[1][0] = M.m[0][1]; MT.m[1][1] = M.m[1][1]; MT.m[1][2] = M.m[2][1]; MT.m[1][3] = M.m[3][1];
		MT.m[2][0] = M.m[0][2]; MT.m[2][1] = M.m[1][2]; MT.m[2][2] = M.m[2][2]; MT.m[2][3] = M.m[3][2];
		MT.m[3][0] = M.m[0][3]; MT.m[3][1] = M.m[1][3]; MT.m[3][2] = M.m[2][3]; MT.m[3][3] = M.m[3][3];
		return MT;
	}
};