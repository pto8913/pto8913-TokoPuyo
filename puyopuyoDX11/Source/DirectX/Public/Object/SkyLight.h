#pragma once

#include "LightBase.h"

class SkyLight : public LightBase
{
public:
	SkyLight(DirectX11& dx);

	void Reset();
	virtual void ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext) override;

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
	std::shared_ptr<PixelConstantBuffer<ConstantBufferSkyLight>> m_pPixelConstantBuffer;
};