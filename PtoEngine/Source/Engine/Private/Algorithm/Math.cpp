
#include "Algorithm/Math.h"

#if _DEBUG
#include <Windows.h>
#include <format>
#endif

namespace Math
{
	float fabs(float x)
	{
		return x < 0 ? -x : x;
	}
	template<>
	bool IsNearlyEqual<float>(const float& a, const float& b, float epsilon)
	{
		return Math::fabs(a - b) < epsilon;
	}
	DirectX::XMFLOAT2 AddXMF2(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
	{
		return {
			a.x + b.x,
			a.y + b.y
		};
	}
}