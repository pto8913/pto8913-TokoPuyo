
#include "Helper/VectorHelper.h"

#include "Math/Rotator.h"

namespace Vector
{
	FVector Normalize(FVector in)
	{
		return in.Normalize();
	}
	float DotProduct(const FVector& a, const FVector& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	FVector CrossProduct(const FVector& a, const FVector& b)
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
	FVector RotateVector(FVector Target, float yaw, float pitch, float roll)
	{
		FQuaternion Quat = FQuaternion(yaw, pitch, roll);
		FQuaternion Quatinv = Quat.Inverse();
		FQuaternion _vector = FQuaternion(Target.x, Target.y, Target.z, 0.f);

		FQuaternion Rotated = Quat * _vector;
		return {
			Rotated.x,
			Rotated.y,
			Rotated.z
		};
	}

	FVector Clamp(FVector v, float inMin, float inMax)
	{
		float length = v.Length();
		float mulp = 1.f;
		if (length > inMax)
		{
			mulp = inMax / length;
		}
		return v * mulp;
	}

	DirectX::XMFLOAT2 ConvertVector2DToDX(const FVector2D& inVector)
	{
		return {
			inVector.x,
			inVector.y
		};
	}
	FVector2D ConvertDXToVector2D(const DirectX::XMFLOAT2& inVector)
	{
		return {
			inVector.x,
			inVector.y
		};
	}
}
