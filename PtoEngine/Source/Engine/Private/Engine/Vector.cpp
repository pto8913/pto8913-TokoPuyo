
#include "Engine/Vector.h"
#include "Math/Rotator.h"

#include <format>

// --------------------------------------------------------------------------------------------------
// Vector 2D
// --------------------------------------------------------------------------------------------------
FVector2D::FVector2D()
	: x(0.f), y(0.f)
{
}
FVector2D::FVector2D(const float& inX, const float& inY)
	: x(inX), y(inY)
{
}

FVector2D FVector2D::operator+(const FVector2D& in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator+(const float& in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator-(const FVector2D & in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator-(const float& in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator*(const FVector2D& in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator*(const float& in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator/(const FVector2D& in)
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator/(const float& in)
{
	FVector2D Out = *this;
	return Out += in;
};

FVector2D& FVector2D::operator+=(const FVector2D& in)
{
	x += in.x;
	y += in.y;
	return *this;
}
FVector2D& FVector2D::operator+=(const float& in)
{
	x += in;
	y += in;
	return *this;
}
FVector2D& FVector2D::operator-=(const FVector2D & in)
{
	x -= in.x;
	y -= in.y;
	return *this;
}
FVector2D& FVector2D::operator-=(const float& in)
{
	x -= in;
	y -= in;
	return *this;
}
FVector2D& FVector2D::operator*=(const FVector2D & in)
{
	x *= in.x;
	y *= in.y;
	return *this;
}
FVector2D& FVector2D::operator*=(const float& in)
{
	x *= in;
	y *= in;
	return *this;
}
FVector2D& FVector2D::operator/=(const FVector2D& in)
{
	x /= in.x;
	y /= in.y;
	return *this;
}
FVector2D& FVector2D::operator/=(const float& in)
{
	x /= in;
	y /= in;
	return *this;
}

std::string FVector2D::ToString() const
{
	return std::format("x: {}, y: {}", x, y);
};

float FVector2D::Length() const
{
	return sqrt((x * x) + (y * y));
}

FVector2D FVector2D::Normalize()
{
	float normLength = 1.f / Length();
	FVector2D Out = *this;
	Out *= normLength;
	return Out;
}

// --------------------------------------------------------------------------------------------------
// Vector
// --------------------------------------------------------------------------------------------------
FVector::FVector() 
	: x(0.f), y(0.f), z(0.f)
{
}
FVector::FVector(const float& inX, const float& inY, const float& inZ)
	: x(inX), y(inY), z(inZ)
{
}

FVector FVector::operator+(const FVector & in)
{
	FVector Out = *this;
	return Out += in;
};
FVector FVector::operator+(const float& in)
{
	FVector Out = *this;
	return Out += in;
};
FVector FVector::operator-(const FVector& in)
{
	FVector Out = *this;
	return Out -= in;
};
FVector FVector::operator-(const float& in)
{
	FVector Out = *this;
	return Out -= in;
};
FVector FVector::operator*(const FVector& in)
{
	FVector Out = *this;
	return Out *= in;
};
FVector FVector::operator*(const float& in)
{
	FVector Out = *this;
	return Out *= in;
};
FVector FVector::operator/(const FVector& in)
{
	FVector Out = *this;
	return Out /= in;
};
FVector FVector::operator/(const float& in)
{
	FVector Out = *this;
	return Out /= in;
};

FVector& FVector::operator+=(const FVector & in)
{
	this->x += in.x;
	this->y += in.y;
	this->z += in.z;
	return *this;
};
FVector& FVector::operator+=(const float& in)
{
	this->x += in;
	this->y += in;
	this->z += in;
	return *this;
};
FVector& FVector::operator-=(const FVector & in)
{
	this->x -= in.x;
	this->y -= in.y;
	this->z -= in.z;
	return *this;
};
FVector& FVector::operator-=(const float& in)
{
	this->x -= in;
	this->y -= in;
	this->z -= in;
	return *this;
};
FVector& FVector::operator*=(const FVector & in)
{
	this->x *= in.x;
	this->y *= in.y;
	this->z *= in.z;
	return *this;
};
FVector& FVector::operator*=(const float& in)
{
	this->x *= in;
	this->y *= in;
	this->z *= in;
	return *this;
};
FVector& FVector::operator/=(const FVector & in)
{
	this->x /= in.x;
	this->y /= in.y;
	this->z /= in.z;
	return *this;
};
FVector& FVector::operator/=(const float& in)
{
	this->x /= in;
	this->y /= in;
	this->z /= in;
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
}