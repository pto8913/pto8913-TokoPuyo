
#include "Render/Factory/IndexBuffer.h"
#include "Render/Manager/BindableManager.h"

#include "Utils/String.h"

IndexBuffer::IndexBuffer(DirectX11& dx, std::string inTag, const std::vector<DWORD>& pInitData)
	: count((UINT)pInitData.size())
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth =           (UINT)(count * sizeof(DWORD));
	desc.Usage =               D3D11_USAGE_DEFAULT;
	desc.BindFlags =           D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags =      0;
	desc.MiscFlags =           0;
	desc.StructureByteStride = sizeof(DWORD);

	//std::string str = std::string("IndexBuffer Size :") + std::to_string(desc.ByteWidth);
	//MessageBox(NULL, Util::s2WString(str).c_str(), L"IndexBuffer Error", MB_OK);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = pInitData.data();

	HRESULT result = GetDevice(dx)->CreateBuffer(
		&desc, 
		&data, 
		&m_pIndexBuffer
	);
	if (FAILED(result))
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
	GetContext(dx)->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
UINT IndexBuffer::GetCount() const
{
	return count;
}