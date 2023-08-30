
#include "Helper/RectHelper.h"
#include "Engine/Vector.h"

D2D1_RECT_F RectHelper::ConvertRectToD2D(const FRect& in)
{
	return D2D1::RectF(
		in.left, 
		in.top, 
		in.right, 
		in.bottom
	);
}
FRect RectHelper::ConvertD2DToRect(const D2D1_RECT_F& in)
{
	return FRect(
		in.left,
		in.top,
		in.right,
		in.bottom
	);
}

bool RectHelper::IsInRect(const FRect& a, const FRect& b)
{
	return (
		(a.left <= b.left && b.left <= a.right) ||
		(a.left <= b.right && b.right <= a.right) ||
		(a.top <= b.top && b.top <= a.bottom) ||
		(a.top <= b.bottom && b.bottom <= a.bottom)
		);
}
bool RectHelper::IsPointInRect(const FRect& a, const FVector& b)
{
	return (
		(a.left <= b.x && b.x <= a.right) &&
		(a.top <= b.y && b.y <= a.bottom)
	);
}