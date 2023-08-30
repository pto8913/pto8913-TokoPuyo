#pragma once

#include "Bindable.h"

class DirectX11;

class Texture : public Bindable
{
public:
	enum TextureType
	{
		WIC,
		DDS
	};
public:
	Texture(DirectX11& dx, TextureType inTextureType, const std::wstring& inFileName, UINT inSlot = 0);
	~Texture();

	static std::shared_ptr<Texture> Make(DirectX11& dx, TextureType inTextureType, const std::wstring& inFileName, UINT inSlot = 0);

	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(TextureType inTextureType, const std::wstring& inFileName, UINT slot = 0)
	{
		inTextureType; inFileName;
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + std::to_string(slot);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(textureType, fileName, slot);
	}
protected:
	TextureType textureType;
	std::wstring fileName;
	UINT slot;

	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureView;
};
