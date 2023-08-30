
#include "Engine/Rotator.h"
#include <math.h>

// -----------------------------------------------------------------------------------
// Quaternion
// -----------------------------------------------------------------------------------
FQuaternion::FQuaternion(float inX, float inY, float inZ, float inW)
	: x(inX), y(inY), z(inZ), w(inW)
{
}
FQuaternion::FQuaternion(float yaw, float pitch, float roll)
{
	float cr = (float)cos(roll / 2.f);
	float cp = (float)cos(pitch / 2.f);
	float cy = (float)cos(yaw / 2.f);

	float sr = (float)sin(roll / 2.f);
	float sp = (float)sin(pitch / 2.f);
	float sy = (float)sin(yaw / 2.f);

	x = sr * cp * cy - cr * sp * sy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
	w = cr * cp * cy + sr * sp * sy;
}

FQuaternion FQuaternion::operator*(const FQuaternion& in)
{
	FQuaternion Out;
	Out.x =  w * in.x - z * in.y + y * in.z + x * in.w;
	Out.y =  z * in.x + w * in.y - x * in.z + y * in.w;
	Out.z = -y * in.x + x * in.y + w * in.z + z * in.w;
	Out.w = -x * in.x - y * in.y - z * in.z + w * in.w;
	return Out;
}

FQuaternion FQuaternion::Inverse()
{
	return { -x, -y, -z, w };
}

// -----------------------------------------------------------------------------------
// Rotator
// -----------------------------------------------------------------------------------
FRotator::FRotator(const float& inYaw, const float& inPitch, const float& inRoll)
	: roll(inYaw), pitch(inPitch), yaw(inRoll)
{
}
FRotator::FRotator(const FQuaternion& in)
{
	float t0 = 2.f * (in.w * in.x + in.z * in.y);
	float t1 = 1.f - 2.f * (in.x * in.x + in.y * in.y);
	roll = (float)atan2(t0, t1);

	t0 = 2.f * (in.w * in.y - in.z * in.x);
	t0 = t0 >  1.f ?  1.f : t0;
	t0 = t0 < -1.f ? -1.f : t0;
	pitch = (float)atan(t0);

	t0 = 2.f * (in.w * in.z + in.x * in.y);
	t1 = 1.f - 2.f * (in.z * in.z + in.y * in.y);
	yaw = (float)atan2(t0, t1);
}

namespace Quaternion
{
	FQuaternion SimpleAdd(FQuaternion a, FQuaternion b)
	{
		return {
			a.x + b.x,
			a.y + b.y,
			a.z + b.z,
			a.w + b.w
		};
	}
	FQuaternion SimpleSub(FQuaternion a, FQuaternion b)
	{
		return {
			a.x - b.x,
			a.y - b.y,
			a.z - b.z,
			a.w - b.w
		};
	}
	FQuaternion SimpleMultiply(FQuaternion a, FQuaternion b)
	{
		return {
			a.x * b.x,
			a.y * b.y,
			a.z * b.z,
			a.w * b.w
		};
	}

};