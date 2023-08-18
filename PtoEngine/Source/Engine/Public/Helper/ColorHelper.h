#pragma once

#include "Engine/Color.h"
#include <d2d1.h>

class ColorHelper
{
public:
	static D2D1_COLOR_F ConvertColorToD2D(const FColor& color);
	static FColor ConvertD2DToColor(const D2D1_COLOR_F& color);
};