#pragma once

#include "Bindable.h"

class DirectX11;

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(DirectX11& dx, UINT inSlot = 0u);
	ConstantBuffer(DirectX11& dx, const C& consts, UINT inSlot = 0u);

	void Update(DirectX11& dx, const C& consts);

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
	virtual void Bind(DirectX11& dx) override;
	static std::shared_ptr<VertexConstantBuffer<C>> Make(DirectX11& dx, const C& consts, UINT inSlot = 0u);
	static std::shared_ptr<VertexConstantBuffer<C>> Make(DirectX11& dx, UINT inSlot = 0u);

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
	virtual void Bind(DirectX11& dx) override;
	static std::shared_ptr<PixelConstantBuffer<C>> Make(DirectX11& dx, const C& consts, UINT inSlot = 0u);
	static std::shared_ptr<PixelConstantBuffer<C>> Make(DirectX11& dx, UINT inSlot = 0u);

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
	virtual void Bind(DirectX11& dx) override;

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
