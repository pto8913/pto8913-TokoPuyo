#pragma once

namespace Algo
{
	/*
	* Need to operator*(float) overload.
	* T operator*(float other) const
	* T operator*(const T& other) const
	*/
	template<typename T, typename U>
	T lerp(const T& a, const T& b, const U& alpha /*0.f ~ 1.f*/)
	{
		return a * (1.f - alpha) + (b * alpha);
	}
}