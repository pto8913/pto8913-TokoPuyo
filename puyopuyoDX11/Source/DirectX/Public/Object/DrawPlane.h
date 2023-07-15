#pragma once

#include "DrawableObject3D.h"

class DirectX11;

class DrawPlane : public DrawableObject3D
{
public:
	DrawPlane(DirectX11& dx, float size);
};