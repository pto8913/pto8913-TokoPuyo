#pragma once

#include "TriangleList.h"

class Plane
{
public:
	static TriangleList Make();
	static TriangleList Make(float size, float u = 1.f, float v = 1.f);
	// NOTE : division >= 1.
	static TriangleList MakeTesselated(UINT division_x = 1, UINT division_y = 1);

	static TriangleList2D Make2D();
	static TriangleList2D Make2D(float size, float u = 1.f, float v = 1.f);
	// NOTE : division >= 1.
	static TriangleList2D MakeTesselated2D(UINT division_x = 1, UINT division_y = 1);
};