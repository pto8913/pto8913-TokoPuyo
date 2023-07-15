#pragma once

#include "Bindable.h"
#include "Render/Manager/BindableManager.h"

#include "Math/Vertex.h"

class DirectX11;

template<typename StructType>
class VertexBuffer : public Bindable
{
public:
	VertexBuffer(DirectX11& dx, std::string inTag, const std::vector<StructType>& pInitData)
		: tag(inTag), stride((UINT)pInitData.size())
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = (UINT)pInitData.size() * sizeof(StructType);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

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
	~VertexBuffer()
	{
		Util::SafeRelease(m_pVertexBuffer);
	}

	static std::shared_ptr<VertexBuffer<StructType>> Make(
		DirectX11& dx,
		std::string inTag,
		const std::vector<StructType>& pInitData
	)
	{
		return BindableManager::Make<VertexBuffer<StructType>>(dx, inTag, pInitData);
	}

	virtual void Bind(DirectX11&, ID3D11DeviceContext* pContext) override
	{
		const UINT _stride = sizeof(StructType);
		const UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_stride, &offset);
	}

	static std::string GenerateID(const std::string& tag, const std::vector<StructType>& pInitData)
	{
		pInitData;
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
	std::string GetUID() const noexcept
	{
		return GenerateID(tag, {});
	}
protected:
	ID3D11Buffer* m_pVertexBuffer;
	std::string tag;
	UINT stride;
};