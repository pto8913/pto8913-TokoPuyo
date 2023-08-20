#pragma once

#include "Engine/Rect.h"
#include <d2d1.h>

class RectHelper
{
public:
	static D2D1_RECT_F ConvertRectToD2D(const FRect& in);
	static FRect ConvertD2DToRect(const D2D1_RECT_F& in);
};