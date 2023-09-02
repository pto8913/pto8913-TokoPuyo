
#include "Math/Vector.h"
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
FVector2D::FVector2D(float&& inX, float&& inY)
	: x(std::move(inX)), y(std::move(inY))
{
}

FVector2D FVector2D::operator+(const FVector2D& in) const
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator+(const float& in) const
{
	FVector2D Out = *this;
	return Out += in;
};
FVector2D FVector2D::operator-(const FVector2D& in) const
{
	FVector2D Out = *this;
	return Out -= in;
};
FVector2D FVector2D::operator-(const float& in) const
{
	FVector2D Out = *this;
	return Out -= in;
};
FVector2D FVector2D::operator*(const FVector2D& in) const
{
	FVector2D Out = *this;
	return Out *= in;
};
FVector2D FVector2D::operator*(const float& in) const
{
	FVector2D Out = *this;
	return Out *= in;
};
FVector2D FVector2D::operator/(const FVector2D& in) const
{
	FVector2D Out = *this;
	return Out /= in;
};
FVector2D FVector2D::operator/(const float& in) const
{
	FVector2D Out = *this;
	return Out /= in;
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
FVector::FVector(float&& inX, float&& inY, float&& inZ)
	: x(std::move(inX)), y(std::move(inY)), z(std::move(inZ))
{
}
FVector FVector::operator+(const FVector & in) const
{
	FVector Out = *this;
	return Out += in;
};
FVector FVector::operator+(const float& in) const
{
	FVector Out = *this;
	return Out += in;
};
FVector FVector::operator-(const FVector& in) const
{
	FVector Out = *this;
	return Out -= in;
};
FVector FVector::operator-(const float& in) const
{
	FVector Out = *this;
	return Out -= in;
};
FVector FVector::operator*(const FVector& in) const
{
	FVector Out = *this;
	return Out *= in;
};
FVector FVector::operator*(const float& in) const
{
	FVector Out = *this;
	return Out *= in;
};
FVector FVector::operator/(const FVector& in) const
{
	FVector Out = *this;
	return Out /= in;
};
FVector FVector::operator/(const float& in) const
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

bool FVector::operator<(const float& in)
{
	return Length() < in;
}

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
FVector2D FVector::To2D() const
{
	return FVector2D(x, y);
}
FVector FVector::Abs() const
{
	FVector out(*this);
	out.x = fabs(out.x);
	out.y = fabs(out.y);
	out.z = fabs(out.z);
	return out;
}

const FVector FVector::Zero = FVector();

