#pragma once

#include "DrawableObject3D.h"

class DirectX11;

class DrawPyramid : public DrawableObject3D
{
public:
	DrawPyramid(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ);
	void SetVertices(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ);
};