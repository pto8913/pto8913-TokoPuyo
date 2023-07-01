
#include "Object/PointLight.h"
#include "Object/DrawSphere.h"

PointLight::PointLight(DirectX11& dx, float radius)
{
	m_pDrawSphere = new DrawSphere(dx, radius);
	m_pPixelConstantBuffer = PixelConstantBuffer<ConstantBufferPointLight>::Make(dx);

	defaultLight = {
		{ 0, 0, 0 },
		{ 0.05f, 0.05f, 0.05f },
		{ 1.f,   1.f,   1.f,  },
		1.f
	};

}

void PointLight::Reset()
{
	currentLight = defaultLight;
}
void PointLight::ExecuteTasks(DirectX11& dx)
{
	m_pDrawSphere->SetLocation(currentLight.position);
	m_pDrawSphere->ExecuteTasks(dx);
}