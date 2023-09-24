#pragma once

#include "TriangleList.h"
#include "Graphics/DirectXHead.h"

class Sphere
{
public:
	static TriangleList MakeTesselated(int latDiv, int longDiv);

	static TriangleList Make();
};