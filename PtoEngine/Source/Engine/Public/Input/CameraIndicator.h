#pragma once

#include "Graphics/Object/DrawableObject3D.h"

class DirectX11;

class CameraIndicator : public DrawableObject3D
{
public:
	CameraIndicator(DirectX11& dx);
};