
#include "Helper/ColorHelper.h"

D2D1_COLOR_F ColorHelper::ConvertColorToD2D(const FColor& color)
{
	return D2D1::ColorF(
		color.r, color.g, color.b, color.a
	);
}

FColor ColorHelper::ConvertD2DToColor(const D2D1_COLOR_F& color)
{
	return FColor(
		color.r, color.g, color.b, color.a
	);
}
