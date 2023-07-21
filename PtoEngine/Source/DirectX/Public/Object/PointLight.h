#pragma once

#include "LightBase.h"

class DirectX11;
class DrawSphere;

class PointLight : public LightBase
{
public:
	PointLight(DirectX11& dx, DirectX::XMVECTOR location, float radius = 0.5f);

	void Reset();
	virtual void ExecuteTasks(DirectX11& dx);

	struct ConstantBufferPointLight
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR ambient;
		DirectX::XMVECTOR diffuse;
		float difIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
protected:
	ConstantBufferPointLight defaultLight;
	ConstantBufferPointLight currentLight;
	DrawSphere* m_pDrawSphere;
	std::shared_ptr<PixelConstantBuffer<ConstantBufferPointLight>> m_pPixelConstantBuffer;
};