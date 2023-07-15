
#include "Render/Factory/ViewPort.h"

ViewPort::ViewPort(float width, float height)
{
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
}

void ViewPort::Bind(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	pContext->RSSetViewports(1, &viewport);
}