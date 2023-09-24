
#include "Graphics/Render/ViewPort.h"

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

void ViewPort::Bind(DirectX11& dx)
{
	GetContext(dx)->RSSetViewports(1, &viewport);
}