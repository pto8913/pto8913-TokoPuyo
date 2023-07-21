
#include "Math/Math.h"

namespace Math
{
	DirectX::XMFLOAT2 AddXMF2(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
	{
		return {
			a.x + b.x,
			a.y + b.y
		};
	}
}