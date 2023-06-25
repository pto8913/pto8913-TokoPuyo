
#include "Render/Viewport/ViewPort.h"

ViewPort::~ViewPort()
{
	Util::SafeRelease(m_pWireFrame);
	Util::SafeRelease(m_pCCWcull);
	Util::SafeRelease(m_pCWcull);

	Util::SafeRelease(m_pNoCull);
}

void ViewPort::CreateViewPort(ID3D11DeviceContext* pID3DContext, float width, float height)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	pID3DContext->RSSetViewports(1, &viewport);
}

void ViewPort::SetModeWireFrame(ID3D11Device* pID3DDevice, ID3D11DeviceContext* pID3DContext)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;

	HRESULT result = pID3DDevice->CreateRasterizerState(
		&desc, &m_pWireFrame
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not set WireFrame", L"Failed WireFrame", MB_OK);
		assert(false);
	}
	pID3DContext->RSSetState(m_pWireFrame);
}
void ViewPort::SetModeTransparent(ID3D11Device* pID3DDevice)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;

	desc.FrontCounterClockwise = true;
	HRESULT result = pID3DDevice->CreateRasterizerState(
		&desc, &m_pCCWcull
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not set CCWcull", L"Failed CCWcull", MB_OK);
		assert(false);
	}
	desc.FrontCounterClockwise = false;
	result = pID3DDevice->CreateRasterizerState(
		&desc, &m_pCWcull
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not set CWcull", L"Failed CWcull", MB_OK);
		assert(false);
	}
}
void ViewPort::SetModePixelClipping(ID3D11Device* pID3DDevice)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;

	HRESULT result = pID3DDevice->CreateRasterizerState(
		&desc, &m_pNoCull
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not set NoCull", L"Failed NoCull", MB_OK);
		assert(false);
	}
}

void ViewPort::GetTransparentBuffer(ID3D11RasterizerState** OutCCWcull, ID3D11RasterizerState** OutCWcull)
{
	*OutCCWcull = m_pCCWcull;
	*OutCWcull = m_pCWcull;
}