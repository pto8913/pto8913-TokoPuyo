
#include "Render/Factory/ConstantBuffer.h"

ConstantBufferEx::ConstantBufferEx(DirectX11& dx, UINT size, UINT inSlot)
	: slot(inSlot)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	//std::string str = std::string("ConstantBufferEx Size :") + std::to_string(desc.ByteWidth);
	//MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBufferEx", MB_OK);

	HRESULT result = GetDevice(dx)->CreateBuffer(
		&desc,
		NULL,
		&m_pConstantBuffer
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ConstantBufferEx", L"Failed ConstantBufferEx", MB_OK);
		assert(false);
	}
}
std::shared_ptr<ConstantBufferEx> ConstantBufferEx::Make(DirectX11& dx, UINT size, UINT inSlot)
{
	return BindableManager::Make<ConstantBufferEx>(dx, size, inSlot);
}
void ConstantBufferEx::Bind(DirectX11&, ID3D11DeviceContext* pContext, void* pData)
{
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &pData, 0, 0);
}