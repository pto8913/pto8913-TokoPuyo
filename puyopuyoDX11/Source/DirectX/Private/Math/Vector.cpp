
#include "Math/Vector.h"
#include "Math/Rotator.h"

#include <format>

FVector::FVector() 
	: x(0.f), y(0.f), z(0.f)
{
}

FVector::FVector(const float& inX, const float& inY, const float& inZ)
	: x(inX), y(inY), z(inZ)
{
}

FVector FVector::operator + (const FVector & In)
{
	FVector Out;
	return Out += In;
};
FVector& FVector::operator += (const FVector & In)
{
	this->x += In.x;
	this->y += In.y;
	this->z += In.z;
	return *this;
};

FVector FVector::operator + (const float& In)
{
	FVector Out;
	return Out += In;
};
FVector& FVector::operator += (const float& In)
{
	this->x += In;
	this->y += In;
	this->z += In;
	return *this;
};

FVector FVector::operator - (const FVector& In)
{
	FVector Out;
	return Out -= In;
};
FVector& FVector::operator -= (const FVector & In)
{
	this->x -= In.x;
	this->y -= In.y;
	this->z -= In.z;
	return *this;
};

FVector FVector::operator - (const float& In)
{
	FVector Out;
	return Out -= In;
};
FVector& FVector::operator -= (const float& In)
{
	this->x -= In;
	this->y -= In;
	this->z -= In;
	return *this;
};

FVector FVector::operator* (const FVector& In)
{
	FVector Out;
	return Out *= In;
};
FVector& FVector::operator *= (const FVector & In)
{
	this->x *= In.x;
	this->y *= In.y;
	this->z *= In.z;
	return *this;
};

FVector FVector::operator* (const float& In)
{
	FVector Out;
	return Out *= In;
};
FVector& FVector::operator *= (const float& In)
{
	this->x *= In;
	this->y *= In;
	this->z *= In;
	return *this;
};

FVector FVector::operator / (const FVector& In)
{
	FVector Out;
	return Out /= In;
};
FVector& FVector::operator /= (const FVector & In)
{
	this->x /= In.x;
	this->y /= In.y;
	this->z /= In.z;
	return *this;
};

FVector FVector::operator/(const float& In)
{
	FVector Out;
	return Out /= In;
};
FVector& FVector::operator/=(const float& In)
{
	this->x /= In;
	this->y /= In;
	this->z /= In;
	return *this;
};

std::string FVector::ToString() const
{
	return std::format("x: {}, y: {}, z: {}", x, y, z);
};

float FVector::Length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

FVector FVector::Normalize()
{
	float normLength = 1.f / Length();
	FVector Out = *this;
	Out *= normLength;
	return Out;
}

namespace Vector
{
	FVector Normalize(FVector In)
	{
		return In.Normalize();
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
		FQuaternion QuatInv = Quat.Inverse();
		FQuaternion _vector = FQuaternion(Target.x, Target.y, Target.z, 0.f);

		FQuaternion Rotated = Quat * _vector;
		return {
			Rotated.x,
			Rotated.y,
			Rotated.z
		};
	}
}