#pragma once

#include <math.h>
#include <algorithm>
#include <DirectXMath.h>
#include <basetsd.h>

constexpr float PI = 3.14159265f;
constexpr DirectX::XMVECTOR WorldForwardVector = { 0, 0, 1, 0 };
constexpr DirectX::XMVECTOR WorldRightVector   = { 1, 0, 0, 0 };
constexpr DirectX::XMVECTOR WorldUpVector      = { 0, 1, 0, 0 };

namespace Math
{
	template<typename T>
	T MapRange(T value, T input_start, T input_end, T output_start, T output_end)
	{
		double slope = 1.0 * (output_end - output_start) / (input_end - input_start);
		return output_start + (T)round(slope * (value - input_start));
	}

	template<typename T>
	T wrap_angle(T theta) noexcept
	{
		const T PI2 = 2 * PI;
		const T mod = (T)fmod(theta, PI2);
		if (mod > (T)PI)
		{
			return mod - PI2;
		}
		else if (mod < -(T)PI)
		{
			return mod + PI2;
		}
		return mod;
	}

	template<typename T>
	constexpr T Clamp(const T& t, const T& inMin, const T& inMax)
	{
		if (t < inMin)
		{
			return inMin;
		}
		else if (t > inMax)
		{
			return inMax;
		}
		return t;
	}

	float fabs(float x);

	/*
	* Need to operator-(T) overload.
	* T operator-(const T& other) const
	* bool operator<(float) const
	* T Abs()
	*/
	template<typename T>
	bool IsNearlyEqual(const T& a, const T& b, float epsilon = 0.001f)
	{
		return (T)(a - b).Abs() < epsilon;
	}
	template<>
	bool IsNearlyEqual<float>(const float& a, const float& b, float epsilon);

	/*
	* Need to operator*(float) overload.
	* T operator*(float other) const
	* T operator*(const T& other) const
	*/
	template<typename T, typename U>
	T lerp(const T& a, const T& b, const U& alpha /*0.f ~ 1.f*/)
	{
		if (Math::IsNearlyEqual(a, b))
		{
			return a;
		}
		return a * (1.f - alpha) + (b * alpha);
	}

	DirectX::XMFLOAT2 AddXMF2(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b);
}

