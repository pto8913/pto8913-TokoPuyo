#pragma once

#include "LightBase.h"

class SkyLight : public LightBase
{
public:
	SkyLight(DirectX11& dx);

	void Reset();
	void ExecuteTasks(DirectX11& dx);
	
	struct ConstantBufferSkyLight : public LightBuffer
	{
		DirectX::XMVECTOR direction;
	};
private:
	ConstantBufferSkyLight defaultLight;
	ConstantBufferSkyLight currentLight;
	std::shared_ptr<PixelConstantBuffer<ConstantBufferSkyLight>> m_pPixelConstantBuffer;
};