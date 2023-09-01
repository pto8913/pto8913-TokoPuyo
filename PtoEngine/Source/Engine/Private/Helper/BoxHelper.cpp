
#include "Helper/BoxHelper.h"

namespace Box
{
	bool IsInBox(const FBox& a, const FBox& b)
	{
		return IsPointInBox(a, b.leftTop) || IsPointInBox(a, b.rightBottom);
	}
	bool IsPointInBox(const FBox& a, const FVector& b)
	{
		return (a.leftTop.x <= b.x && b.x <= a.rightBottom.x) &&
			(a.leftTop.y <= b.y && b.y <= a.rightBottom.y) &&
			(a.leftTop.z >= b.z && b.z >= a.rightBottom.z);
	}
}
