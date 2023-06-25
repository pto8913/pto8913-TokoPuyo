#pragma once

#include "Object/DrawableObject.h"

class DrawCube : public DrawableObject
{
public:
	DrawCube() = default;
	DrawCube(DirectX11& dx,float size);

};
