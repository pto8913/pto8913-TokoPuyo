
#include "Graphics/Object/SkyLight.h"

SkyLight::SkyLight(DirectX11& dx)
{

	defaultLight.position     = { 0.f, 0.f, 0.f};
	defaultLight.direction    = { 0.f, 1.f, 0.f, 0.f };
	defaultLight.diffuse      = { 1.f, 1.f, 1.f, 1.f };
	defaultLight.difIntensity = 1.f;
	defaultLight.ambient      = { 1.f, 1.f, 1.f, 1.f };

	Reset();

	pPixelConstantBuffer = PixelConstantBuffer<ConstantBufferSkyLight>::Make(dx, defaultLight);
}
SkyLight::~SkyLight()
{
	pPixelConstantBuffer.reset();
	pPixelConstantBuffer = nullptr;
}

void SkyLight::Reset()
{
	currentLight = defaultLight;
}
void SkyLight::ExecuteTasks(DirectX11& dx)
{
	currentLight.position = DirectX::XMVector3Transform(
		currentLight.position,
		dx.GetCameraView()
	);
	pPixelConstantBuffer->Update(dx.GetContext(), currentLight);
	pPixelConstantBuffer->Bind(dx);
}