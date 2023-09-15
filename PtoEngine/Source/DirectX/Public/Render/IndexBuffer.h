#pragma once

#include "Bindable.h"

class DirectX11;

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(DirectX11& dx, std::string inTag, const std::vector<DWORD>& pInitData);
	virtual ~IndexBuffer();

	static std::shared_ptr<IndexBuffer> Make(
		DirectX11& dx,
		std::string inTag,
		const std::vector<DWORD>& pInitData
	);

	virtual void Bind(DirectX11& dx) override;
	UINT GetCount() const;

	template<typename... Ignore>
	static std::string GenerateID(const std::string& tag, Ignore&&...)
	{
		return "IndexBuffer_" + tag;
	}
protected:
	ID3D11Buffer* pIndexBuffer = nullptr;
	std::string tag;
	UINT count;
};