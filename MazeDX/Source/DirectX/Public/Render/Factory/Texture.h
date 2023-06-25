#pragma once

#include "Bindable.h"

class DirectX11;

class Texture : public Bindable
{
public:
	Texture(DirectX11& dx, const std::wstring& inFileName, UINT inSlot = 0);
	~Texture();

	static std::shared_ptr<Texture> Make(DirectX11& dx, const std::wstring& inFileName, UINT inSlot = 0);

	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(const std::wstring& inFileName, UINT slot = 0)
	{
		inFileName;
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + std::to_string(slot);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(fileName, slot);
	}
protected:
	UINT slot;
	std::wstring fileName;
	ID3D11ShaderResourceView* m_pTextureView;

	static constexpr DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	DirectX::ScratchImage scratch;
};
