#pragma once

#include "Bindable.h"

#include "Math/Vertex.h"

class DirectX11;

class VertexBuffer : public Bindable
{
public:
	VertexBuffer(DirectX11& dx, std::string inTag, const DX::VertexBuffer& pInitData);
	~VertexBuffer();

	static std::shared_ptr<VertexBuffer> Make(
		DirectX11& dx, 
		std::string inTag, 
		const DX::VertexBuffer& pInitData
	);

	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(const std::string& tag, const DX::VertexBuffer& pInitData)
	{
		pInitData;
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
	std::string GetUID() const noexcept
	{
		return GenerateID(tag, DX::VertexBuffer());
	}
protected:
	ID3D11Buffer* m_pVertexBuffer;
	std::string tag;
	DX::VertexLayout layout;
	UINT stride;
};