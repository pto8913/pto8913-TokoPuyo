
#include "Render/Factory/VertexBuffer.h"
#include "Render/Manager/BindableManager.h"

VertexBuffer::VertexBuffer(DirectX11& dx, std::string inTag, const std::vector<DX::FVertex>& pInitData)
	: tag(inTag), 
	stride((UINT)pInitData.size())
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = (UINT)pInitData.size();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	std::string str = std::string("VertexBuffer Size :") + std::to_string(desc.ByteWidth);
	MessageBox(NULL, Util::s2WString(str).c_str(), L"pVertexBuffer Error", MB_OK);

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = pInitData.data();

	HRESULT result = GetDevice(dx)->CreateBuffer(
		&desc, 
		&vertexBufferData, 
		&m_pVertexBuffer
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"can not create pVertexBuffer", L"pVertexBuffer Error", MB_OK);
		assert(false);
	}
}

VertexBuffer::~VertexBuffer()
{
	Util::SafeRelease(m_pVertexBuffer);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Make(DirectX11& dx, std::string inTag, const std::vector<DX::FVertex>& pInitData)
{
	return BindableManager::Make<VertexBuffer>(dx, inTag, pInitData);
}

void VertexBuffer::Bind(DirectX11& dx)
{
	const UINT offset = 0;
	GetContext(dx)->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
}