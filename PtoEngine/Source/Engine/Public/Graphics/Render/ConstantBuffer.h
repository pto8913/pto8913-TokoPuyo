#pragma once

#include "Bindable.h"
#include "BindableManager.h"

class DirectX11;

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(DirectX11& dx, UINT inSlot = 0u)
		: slot(inSlot)
	{
		D3D11_BUFFER_DESC desc;
		desc.BindFlags =      D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage =          D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags =      0;
		desc.ByteWidth =      sizeof(C);

		//std::string str = std::string("ConstantBuffer Size :") + std::to_string(desc.ByteWidth);
		//MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBuffer Error", MB_OK);

		HRESULT result = GetDevice(dx)->CreateBuffer(
			&desc,
			NULL,
			&pConstantBuffer
		);
		if (FAILED(result))
		{
			MessageBox(NULL, L"Can not create ConstantBuffer", L"Failed ConstantBuffer", MB_OK);
			assert(false);
		}
	};
	ConstantBuffer(DirectX11& dx, const C& consts, UINT inSlot = 0u)
		: slot(inSlot)
	{
		D3D11_BUFFER_DESC desc;
		desc.BindFlags =      D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage =          D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags =      0;
		desc.ByteWidth =      sizeof(consts);

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &consts;

		HRESULT result = GetDevice(dx)->CreateBuffer(
			&desc, 
			&data, 
			&pConstantBuffer
		);
		if (FAILED(result))
		{
			MessageBox(NULL, L"Can not create ConstantBuffer", L"Failed ConstantBuffer", MB_OK);
			assert(false);
		}
	}
	virtual ~ConstantBuffer()
	{
		Util::SafeRelease(pConstantBuffer);
	}

	virtual void Update(ID3D11DeviceContext* pContext, const C& consts)
	{
		pContext->UpdateSubresource(pConstantBuffer, 0, NULL, &consts, 0, 0);
	}
	ID3D11Buffer*& GetBuffer() {
		return pConstantBuffer;
	}
protected:
	ID3D11Buffer* pConstantBuffer = nullptr;
	UINT slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using ConstantBuffer<C>::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;

	virtual void Bind(DirectX11& dx) override 
	{
		GetContext(dx)->VSSetConstantBuffers(slot, 1, &pConstantBuffer);
	}
	static std::shared_ptr<VertexConstantBuffer<C>> Make(DirectX11& dx, const C& consts, UINT inSlot = 0u)
	{
		return BindableManager::Make<VertexConstantBuffer>(dx, consts, inSlot);
	}
	static std::shared_ptr<VertexConstantBuffer<C>> Make(DirectX11& dx, UINT inSlot = 0u)
	{
		return BindableManager::Make<VertexConstantBuffer>(dx, inSlot);
	}

	static std::string GenerateID(const C&, UINT slot)
	{
		return GenerateID(slot);
	}
	static std::string GenerateID(UINT slot = 0)
	{
		return "VertexConstantBuffer_" + std::to_string(slot);
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using ConstantBuffer<C>::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;

	virtual void Bind(DirectX11& dx) override
	{
		GetContext(dx)->PSSetConstantBuffers(slot, 1, &pConstantBuffer);
	}
	static std::shared_ptr<PixelConstantBuffer<C>> Make(DirectX11& dx, const C& consts, UINT inSlot = 0u)
	{
		return BindableManager::Make<PixelConstantBuffer>(dx, consts, inSlot);
	}
	static std::shared_ptr<PixelConstantBuffer<C>> Make(DirectX11& dx, UINT inSlot = 0u)
	{
		return BindableManager::Make<PixelConstantBuffer>(dx, inSlot);
	}

	static std::string GenerateID(const C&, UINT slot)
	{
		return GenerateID(slot);
	}
	static std::string GenerateID(UINT slot = 0)
	{
		return "PixelConstantBuffer_" + std::to_string(slot);
	}
};

class ConstantBufferEx : public Bindable
{
public:
	ConstantBufferEx(
		DirectX11& dx,
		UINT size,
		UINT inSlot = 0
	);
	virtual ~ConstantBufferEx()
	{
		Util::SafeRelease(pConstantBuffer);
	}

	static std::shared_ptr<ConstantBufferEx> Make(DirectX11& dx, UINT size, UINT inSlot = 0);
	virtual void Bind(DirectX11&) override
	{
	};
	virtual void Bind(DirectX11& dx, void* pData);
	
	static std::string GenerateID(UINT size, UINT inSlot)
	{
		return "ConstantBufferEx_" + std::to_string(size) + "_" + std::to_string(inSlot);
	}

	ID3D11Buffer*& GetBuffer()
	{
		return pConstantBuffer;
	}
protected:
	UINT slot;
	ID3D11Buffer* pConstantBuffer = nullptr;
};