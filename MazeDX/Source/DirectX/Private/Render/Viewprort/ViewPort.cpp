
#include "Render/Viewport/ViewPort.h"

ViewPort::ViewPort(DirectX11& dx, float width, float height)
{
	dx;
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