#pragma once

#include "Math/Rect.h"
#include <d2d1.h>

struct FVector;

class RectHelper
{
public:
	static D2D1_RECT_F ConvertRectToD2D(const FRect& in);
	static FRect ConvertD2DToRect(const D2D1_RECT_F& in);
	static bool IsInRect(const FRect& a, const FRect& b);
	static bool IsPointInRect(const FRect& a, const FVector& b);
};