#pragma once

#include "DrawableObject.h"

class DirectX11;

class DrawPyramid : public DrawableObject
{
public:
	DrawPyramid(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ);
	void SetVertices(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ);
};