#pragma once

#include "Bindable.h"
#include "Render/Manager/BindableManager.h"
#include "Utils/String.h"

class DirectX11;

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(DirectX11& dx, UINT inSlot = 0u)
		: slot(inSlot)
	{
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(C);
		desc.StructureByteStride = 0;

		std::string str = std::string("ConstantBuffer Size :") + std::to_string(desc.ByteWidth);
		MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBuffer Error", MB_OK);

		HRESULT result = GetDevice(dx)->CreateBuffer(&desc, nullptr, &m_pConstantBuffer);
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

	void Update(DirectX11& dx, const C& consts)
	{
		GetContext(dx)->UpdateSubresource(m_pConstantBuffer, 0, NULL, &consts, 0, 0);
		//D3D11_MAPPED_SUBRESOURCE resource;
		//GetContext(dx)->Map(
		//	m_pConstantBuffer,
		//	0,
		//	D3D11_MAP_WRITE_DISCARD,
		//	0,
		//	&resource
		//);

		//memcpy(resource.pData, &consts, sizeof(consts));
		//GetContext(dx)->Unmap(m_pConstantBuffer, 0);
	}
protected:
	ID3D11Buffer* m_pConstantBuffer;
	UINT slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using Bindable::GetContext;
	using ConstantBuffer<C>::m_pConstantBuffer;
	using ConstantBuffer<C>::slot;
public:
	VertexConstantBuffer(DirectX11& dx, UINT inSlot = 0u)
		: ConstantBuffer<C>(dx, inSlot)
	{}

	virtual void Bind(DirectX11& dx) override 
	{
		GetContext(dx)->VSSetConstantBuffers(slot, 1, &m_pConstantBuffer);
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
		using namespace std::string_literals;
		return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using Bindable::GetContext;
	using ConstantBuffer<C>::m_pConstantBuffer;
	using ConstantBuffer<C>::slot;
public:
	PixelConstantBuffer(DirectX11& dx, UINT inSlot = 0u)
		: ConstantBuffer<C>(dx, inSlot)
	{}
	virtual void Bind(DirectX11& dx) override
	{
		GetContext(dx)->PSSetConstantBuffers(slot, 1, &m_pConstantBuffer);
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
		using namespace std::string_literals;
		return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
	}
};

class ConstantBufferPerFrame : public Bindable
{
public:
	ConstantBufferPerFrame(
		DirectX11& dx,
		UINT size,
		void* pInitData
	);

	static std::shared_ptr<ConstantBufferPerFrame> Make(DirectX11& dx, UINT size, void* pInitData);
	virtual void Bind(DirectX11& dx) override {
		dx;
	};
	void Bind(DirectX11& dx, void* pData);
	
	static std::string GenerateID(UINT size, void* pInitData)
	{
		pInitData;
		using namespace std::string_literals;
		return typeid(ConstantBufferPerFrame).name() + "#"s + std::to_string(size);
	}

	ID3D11Buffer*& GetBuffer()
	{
		return m_pConstantBuffer;
	}
protected:
	ID3D11Buffer* m_pConstantBuffer;

};
