#pragma once

#include "DirectX/DirectXHead.h"
#include "Render/Factory/ConstantBuffer.h"

class DirectX11;
class DrawSphere;

class PointLight
{
public:
	PointLight(DirectX11& dx, float radius = 0.5f);

	void Reset();
	virtual void ExecuteTasks(DirectX11& dx);

	struct ConstantBufferPointLight
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR ambient;
		DirectX::XMVECTOR diffuse;
		float intensity;
	};
protected:
	ConstantBufferPointLight defaultLight;
	ConstantBufferPointLight currentLight;
	DrawSphere* m_pDrawSphere;
	std::shared_ptr<PixelConstantBuffer<ConstantBufferPointLight>> m_pPixelConstantBuffer;
};