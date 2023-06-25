
#include "Render/Factory/IndexBuffer.h"
#include "Render/Manager/BindableManager.h"

IndexBuffer::IndexBuffer(DirectX11& dx, std::string inTag, const std::vector<DWORD>& pInitData)
	: count((UINT)pInitData.size())
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = (UINT)(pInitData.size() * sizeof(DWORD));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = sizeof(DWORD);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = pInitData.data();

	if (FAILED(GetDevice(dx)->CreateBuffer(&desc, &data, &m_pIndexBuffer)))
	{
		MessageBox(NULL, L"Can not create IndexBuffer", L"Failed IndexBuffer", MB_OK);
		assert(false);
	}
}
IndexBuffer::~IndexBuffer()
{
	Util::SafeRelease(m_pIndexBuffer);
}

std::shared_ptr<IndexBuffer> IndexBuffer::Make(DirectX11& dx, std::string inTag, const std::vector<DWORD>& pInitData)
{
	return BindableManager::Make<IndexBuffer>(dx, inTag, pInitData);
}

void IndexBuffer::Bind(DirectX11& dx)
{
	GetContext(dx)->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}