
#include "Graphics/Object/PointLight.h"
#include "Graphics/Object/DrawSphere.h"

PointLight::PointLight(DirectX11& dx, DirectX::XMVECTOR location, float radius)
{
	pDrawSphere = new DrawSphere(dx, radius);
	pPixelConstantBuffer = PixelConstantBuffer<ConstantBufferPointLight>::Make(dx);

	defaultLight.position = location;
	defaultLight.diffuse = { 1.f,   1.f,   1.f, };
	defaultLight.difIntensity = 1.f;
	defaultLight.ambient = { 0.05f, 0.05f, 0.05f };
	defaultLight.attConst = 1.f;
	defaultLight.attLin = 0.025f;
	defaultLight.attQuad = 0.003f;
}
PointLight::~PointLight()
{
	pPixelConstantBuffer.reset();
	pPixelConstantBuffer = nullptr;

	delete pDrawSphere;
	pDrawSphere = nullptr;
}

void PointLight::Reset()
{
	currentLight = defaultLight;
}
void PointLight::ExecuteTasks(DirectX11& dx)
{
	currentLight.position = DirectX::XMVector3Transform(
		currentLight.position,
		dx.GetCameraView()
	);
	pPixelConstantBuffer->Update(dx.GetContext(), currentLight);
	pPixelConstantBuffer->Bind(dx);

	pDrawSphere->SetLocation(currentLight.position);
	pDrawSphere->ExecuteTasks(dx);
}