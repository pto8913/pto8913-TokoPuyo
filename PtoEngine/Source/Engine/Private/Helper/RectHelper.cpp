
#include "Helper/RectHelper.h"

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
