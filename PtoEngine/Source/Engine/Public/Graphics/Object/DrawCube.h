#pragma once

#include "Graphics/Object/DrawableObject3D.h"

class DrawCube : public DrawableObject3D
{
public:
	DrawCube() = default;
	DrawCube(DirectX11& dx,float size);

};
