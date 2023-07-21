#pragma once

struct FQuaternion
{
public:
	FQuaternion() = default;
	FQuaternion(float inX, float inY, float inZ, float inW);
	FQuaternion(float yaw, float pitch, float roll);
	
	FQuaternion operator*(const FQuaternion& in);

	FQuaternion Inverse();

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;
};

struct FRotator
{
public:
	FRotator() = default;
	FRotator(const float& inYaw, const float& inPitch, const float& inRoll);
	FRotator(const FQuaternion& in);

	float yaw = 0.f;
	float pitch = 0.f;
	float roll = 0.f;
};

namespace Quaternion
{
	FQuaternion SimpleAdd(FQuaternion a, FQuaternion b);
	FQuaternion SimpleSub(FQuaternion a, FQuaternion b);
	FQuaternion SimpleMultiply(FQuaternion a, FQuaternion b);
};