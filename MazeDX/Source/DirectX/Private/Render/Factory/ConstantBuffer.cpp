
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

	std::string str = std::string("ConstantBuffer Size :") + std::to_string(desc.ByteWidth);
	MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBuffer Error", MB_OK);

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
void ConstantBufferPerFrame::Bind(DirectX11& dx, void* pData)
{
	GetContext(dx)->UpdateSubresource(m_pConstantBuffer, 0, NULL, &pData, 0, 0);
	GetContext(dx)->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}