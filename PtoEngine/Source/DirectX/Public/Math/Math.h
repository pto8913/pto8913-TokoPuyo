#pragma once

#include <math.h>
#include <algorithm>
#include <DirectXMath.h>
#include <basetsd.h>

#include "Vector.h"
#include "Matrix.h"

constexpr float PI = 3.14159265f;
constexpr DirectX::XMVECTOR WorldForwardVector = { 0, 0, 1, 0 };
constexpr DirectX::XMVECTOR WorldRightVector   = { 1, 0, 0, 0 };
constexpr DirectX::XMVECTOR WorldUpVector      = { 0, 1, 0, 0 };

namespace Math
{
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
	T MapRange(T value, T input_start, T input_end, T output_start, T output_end)
	{
		double slope = 1.0 * (output_end - output_start) / (input_end - input_start);
		return output_start + (T)round(slope * (value - input_start));
	}

	DirectX::XMFLOAT2 AddXMF2(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b);
}

