
#include "Render/Factory/ConstantBuffer.h"

#include "Render/Manager/BindableManager.h"

template<typename C>
ConstantBuffer<C>::ConstantBuffer(DirectX11& dx, UINT inSlot)
	: slot(inSlot)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(C);
	desc.StructureByteStride = 0;

	HRESULT result = GetDevice(dx)->CreateBuffer(&desc, nullptr, &m_pConstantBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ConstantBuffer", L"Failed ConstantBuffer", MB_OK);
		assert(false);
	}
}
template<typename C>
ConstantBuffer<C>::ConstantBuffer(DirectX11& dx, const C& consts, UINT inSlot)
	: slot(inSlot)
{
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(consts);
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &consts;

	HRESULT result = GetDevice(dx)->CreateBuffer(&desc, &data, &m_pConstantBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ConstantBuffer", L"Failed ConstantBuffer", MB_OK);
		assert(false);
	}
}
template<typename C>
void ConstantBuffer<C>::Update(DirectX11& dx, const C& consts)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT result = GetContext(dx)->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&resource
	);

	memcpy(resource.pData, &consts, sizeof(consts));
	GetContext(dx)->Unmap(m_pConstantBuffer, 0);
}

template<typename C>
void VertexConstantBuffer<C>::Bind(DirectX11& dx)
{
	GetContext(dx)->VSSetConstantBuffer(slot, 1, &m_pConstantBuffer);
}
template<typename C>
std::shared_ptr<VertexConstantBuffer<C>> VertexConstantBuffer<C>::Make(DirectX11& dx, const C& consts, UINT inSlot)
{
	return BindableManager::Make<VertexConstantBuffer>(dx, consts, inSlot);
}
template<typename C>
std::shared_ptr<VertexConstantBuffer<C>> VertexConstantBuffer<C>::Make(DirectX11& dx, UINT inSlot)
{
	return BindableManager::Make<VertexConstantBuffer>(dx, inSlot);
}

template<typename C>
void PixelConstantBuffer<C>::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetConstantBuffer(slot, 1, &m_pConstantBuffer);
}
template<typename C>
std::shared_ptr<PixelConstantBuffer<C>> PixelConstantBuffer<C>::Make(DirectX11& dx, const C& consts, UINT inSlot)
{
	return BindableManager::Make<PixelConstantBuffer>(dx, consts, inSlot);
}
template<typename C>
std::shared_ptr<PixelConstantBuffer<C>> PixelConstantBuffer<C>::Make(DirectX11& dx, UINT inSlot)
{
	return BindableManager::Make<PixelConstantBuffer>(dx, inSlot);
}

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