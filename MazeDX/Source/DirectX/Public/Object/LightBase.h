#pragma once

#include "DrawableObject.h"

#include "Render/Factory/ConstantBuffer.h"

class LightBase : public DrawableObject
{
protected:
	struct LightBuffer
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR ambient;
		DirectX::XMVECTOR diffuse;
		float difIntensity;
	};
};