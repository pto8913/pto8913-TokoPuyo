#pragma once

#include "LightBase.h"

class SkyLight : public LightBase
{
public:
	SkyLight(DirectX11& dx);
	virtual ~SkyLight();

	void Reset();
	virtual void ExecuteTasks(DirectX11& dx) override;

	struct ConstantBufferSkyLight
	{
		DirectX::XMVECTOR direction;
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR ambient;
		DirectX::XMVECTOR diffuse;
		float difIntensity;
	};
private:
	ConstantBufferSkyLight defaultLight;
	ConstantBufferSkyLight currentLight;
	std::shared_ptr<PixelConstantBuffer<ConstantBufferSkyLight>> pPixelConstantBuffer;
};