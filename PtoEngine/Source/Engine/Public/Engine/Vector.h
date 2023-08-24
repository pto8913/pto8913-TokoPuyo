// --------------------------------------------------------------------------------------------------
// 
// x : front, y : right, z : up
//
// --------------------------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <string>

// --------------------------------------------------------------------------------------------------
// Vector 2D
// --------------------------------------------------------------------------------------------------
struct FVector2D
{
	FVector2D();
	FVector2D(const float& inX, const float& inY);

	FVector2D operator+(const FVector2D& In);
	FVector2D operator+(const float& In);
	FVector2D operator-(const FVector2D& In);
	FVector2D operator-(const float& In);
	FVector2D operator*(const FVector2D& In);
	FVector2D operator*(const float& In);
	FVector2D operator/(const FVector2D& In);
	FVector2D operator/(const float& In);

	FVector2D& operator+=(const FVector2D& In);
	FVector2D& operator+=(const float& In);
	FVector2D& operator-=(const FVector2D& In);
	FVector2D& operator-=(const float& In);
	FVector2D& operator*=(const FVector2D& In);
	FVector2D& operator*=(const float& In);
	FVector2D& operator/=(const FVector2D& In);
	FVector2D& operator/=(const float& In);

	std::string ToString() const;
	float Length() const;
	FVector2D Normalize();

	float x;
	float y;
};

// --------------------------------------------------------------------------------------------------
// Vector
// --------------------------------------------------------------------------------------------------
struct FVector
{
public:
	FVector();
	FVector(const float& inX, const float& inY, const float& inZ);

	FVector operator+(const FVector& In) const;
	FVector operator+(const float& In) const;
	FVector operator-(const FVector& In) const;
	FVector operator-(const float& In) const;
	FVector operator*(const FVector& In) const;
	FVector operator*(const float& In) const;
	FVector operator/(const FVector& In) const;
	FVector operator/(const float& In) const;

	FVector& operator+=(const FVector& In);
	FVector& operator+=(const float& In);
	FVector& operator-=(const FVector& In);
	FVector& operator-=(const float& In);
	FVector& operator*=(const FVector& In);
	FVector& operator*=(const float& In);
	FVector& operator/=(const FVector& In);
	FVector& operator/=(const float& In);

	bool operator<(const float& in);

	std::string ToString() const;
	float Length() const;
	FVector Normalize();
	FVector2D To2D() const;
	FVector Abs() const;

	static const FVector Zero;

	float x;
	float y;
	float z;
};

namespace Vector
{
	FVector Normalize(FVector In);
	float DotProduct(const FVector& a, const FVector& b);
	FVector CrossProduct(const FVector& a, const FVector& b);
	FVector RotateVector(FVector Target, float yaw, float pitch, float roll);
};