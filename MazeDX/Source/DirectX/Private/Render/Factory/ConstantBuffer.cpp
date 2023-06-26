
#include "Render/Factory/ConstantBuffer.h"

ConstantBufferPerFrame::ConstantBufferPerFrame(DirectX11& dx, UINT size, void* pInitData)
{
	pInitData;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT result = GetDevice(dx)->CreateBuffer(
		&desc,
		NULL,
		&m_pConstantBuffer
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ConstantBufferPerFrame", L"Failed ConstantBufferPerFrame", MB_OK);
		assert(false);
	}
}
std::shared_ptr<ConstantBufferPerFrame> ConstantBufferPerFrame::Make(DirectX11& dx, UINT size, void* pInitData)
{
	return BindableManager::Make<ConstantBufferPerFrame>(dx, size, pInitData);
}
void ConstantBufferPerFrame::Bind(DirectX11& dx)
{
	dx;
}