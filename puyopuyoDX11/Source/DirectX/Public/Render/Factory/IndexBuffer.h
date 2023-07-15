#pragma once

#include "Bindable.h"

class DirectX11;

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(DirectX11& dx, std::string inTag, const std::vector<DWORD>& pInitData);
	~IndexBuffer();

	static std::shared_ptr<IndexBuffer> Make(
		DirectX11& dx,
		std::string inTag,
		const std::vector<DWORD>& pInitData
	);

	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;
	UINT GetCount() const;

	template<typename... Ignore>
	static std::string GenerateID(const std::string& tag, Ignore&&...)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
protected:
	ID3D11Buffer* m_pIndexBuffer;
	std::string tag;
	UINT count;
};