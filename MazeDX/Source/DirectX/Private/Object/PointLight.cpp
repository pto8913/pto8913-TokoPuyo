
#include "Object/PointLight.h"
#include "Object/DrawSphere.h"

PointLight::PointLight(DirectX11& dx, DirectX::XMVECTOR location, float radius)
{
	m_pDrawSphere = new DrawSphere(dx, radius);
	m_pPixelConstantBuffer = PixelConstantBuffer<ConstantBufferPointLight>::Make(dx);

	defaultLight.position = location;
	defaultLight.diffuse = { 1.f,   1.f,   1.f,  };
	defaultLight.difIntensity = 1.f;
	defaultLight.ambient = { 0.05f, 0.05f, 0.05f };
	defaultLight.attConst = 1.f;
	defaultLight.attLin = 0.025f;
	defaultLight.attQuad = 0.003f;
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
	m_pPixelConstantBuffer.get()->Update(dx, currentLight);
	m_pPixelConstantBuffer.get()->Bind(dx);

	m_pDrawSphere->SetLocation(currentLight.position);
	m_pDrawSphere->ExecuteTasks(dx);
}