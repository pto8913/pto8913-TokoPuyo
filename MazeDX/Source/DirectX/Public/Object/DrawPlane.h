#pragma once

#include "DrawableObject.h"

class DirectX11;

class DrawPlane : public DrawableObject
{
public:
	DrawPlane(DirectX11& dx, float size);
};