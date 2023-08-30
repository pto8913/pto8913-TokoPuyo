
#include "Algorithm/Math2.h"

namespace Algo
{
	float fabs(float x)
	{
		return x < 0 ? -x : x;
	}
	template<>
	bool IsNearlyEqual<float>(const float& a, const float& b, float epsilon)
	{
		return Algo::fabs(a - b) < epsilon;
	}
}