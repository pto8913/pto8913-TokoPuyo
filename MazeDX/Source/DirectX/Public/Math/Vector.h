// --------------------------------------------------------------------------------------------------
// 
// x : front, y : right, z : up
//
// --------------------------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <string>

struct FVector
{
public:
	FVector();
	FVector(const float& inX, const float& inY, const float& inZ);

	FVector operator+(const FVector& In);
	FVector& operator+=(const FVector& In);

	FVector operator+(const float& In);
	FVector& operator+=(const float& In);

	FVector operator-(const FVector& In);
	FVector& operator-=(const FVector& In);

	FVector operator-(const float& In);
	FVector& operator-=(const float& In);

	FVector operator*(const FVector& In);
	FVector& operator*=(const FVector& In);

	FVector operator*(const float& In);
	FVector& operator*=(const float& In);

	FVector operator/(const FVector& In);
	FVector& operator/=(const FVector& In);

	FVector operator/(const float& In);
	FVector& operator/=(const float& In);

	std::string ToString() const;

	float Length() const;

	FVector Normalize();

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