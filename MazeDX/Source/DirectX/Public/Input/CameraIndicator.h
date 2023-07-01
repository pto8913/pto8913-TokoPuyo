#pragma once

#include "Object/DrawableObject.h"

class DirectX11;

class CameraIndicator : public DrawableObject
{
public:
	CameraIndicator(DirectX11& dx);
};