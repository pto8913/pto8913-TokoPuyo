#pragma once

#include "Geometry/TriangleList.h"
#include "DirectX/DirectXHead.h"

class Sphere
{
public:
	static TriangleList MakeTesselated(int latDiv, int longDiv);

	static TriangleList Make();
};