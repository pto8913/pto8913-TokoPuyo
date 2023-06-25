
#include "Render/Factory/VertexBuffer.h"
#include "Render/Manager/BindableManager.h"

VertexBuffer::VertexBuffer(DirectX11& dx, std::string inTag, const DX::VertexBuffer& pInitData)
	: tag(inTag), layout(pInitData.GetLayout()), stride((UINT)pInitData.GetLayout().Size())
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = (UINT)pInitData.SizeBytes();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = pInitData.GetData();

	HRESULT result = GetDevice(dx)->CreateBuffer(&desc, &vertexBufferData, &m_pVertexBuffer);

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

std::shared_ptr<VertexBuffer> VertexBuffer::Make(DirectX11& dx, std::string inTag, const DX::VertexBuffer& pInitData)
{
	return BindableManager::Make<VertexBuffer>(dx, inTag, pInitData);
}

void VertexBuffer::Bind(DirectX11& dx)
{
	const UINT offset = 0;
	GetContext(dx)->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
}