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
	FVector2D(float&& inX, float&& inY);

	FVector2D operator+(const FVector2D& in) const;
	FVector2D operator+(const float& in) const;
	FVector2D operator-(const FVector2D& in) const;
	FVector2D operator-(const float& in) const;
	FVector2D operator*(const FVector2D& in) const;
	FVector2D operator*(const float& in) const;
	FVector2D operator/(const FVector2D& in) const;
	FVector2D operator/(const float& in) const;

	FVector2D& operator+=(const FVector2D& in);
	FVector2D& operator+=(const float& in);
	FVector2D& operator-=(const FVector2D& in);
	FVector2D& operator-=(const float& in);
	FVector2D& operator*=(const FVector2D& in);
	FVector2D& operator*=(const float& in);
	FVector2D& operator/=(const FVector2D& in);
	FVector2D& operator/=(const float& in);

	bool operator==(const FVector2D& in) const;

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
	FVector(float&& inX, float&& inY, float&& inZ);

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

	bool operator<(const float& in) const;
	bool operator==(const FVector& in) const;

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