#pragma once

namespace Algo
{
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
		if (Algo::IsNearlyEqual(a, b))
		{
			return a;
		}
		return a * (1.f - alpha) + (b * alpha);
	}
}