
#include "Graphics/Render/Texture.h"

#include "Graphics/Render/BindableManager.h"

#include "WICTextureLoader11.h"
#include "DDSTextureLoader11.h"

Texture::Texture(DirectX11& dx, Texture::TextureType inTextureType, const std::wstring& inFileName, UINT inSlot)
	: textureType(inTextureType), fileName(inFileName), slot(inSlot)
{
	HRESULT result;
	switch (inTextureType)
	{
	case DDS:
		result = DirectX::CreateDDSTextureFromFile(
			GetDevice(dx),
			inFileName.c_str(),
			(ID3D11Resource**)&pTexture,
			&pTextureView
		);
		break;
	default:
		result = DirectX::CreateWICTextureFromFile(
			GetDevice(dx),
			inFileName.c_str(),
			(ID3D11Resource**)&pTexture,
			&pTextureView
		);
		break;
	}

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not load Texture", L"Failed Texture constructor", MB_OK);
		assert(false);
	}
}
Texture::~Texture()
{
	Util::SafeRelease(pTexture);
	Util::SafeRelease(pTextureView);
}

std::shared_ptr<Texture> Texture::Make(DirectX11& dx, Texture::TextureType inTextureType, const std::wstring& inFileName, UINT inSlot)
{
	return BindableManager::Make<Texture>(dx, inTextureType, inFileName, inSlot);
}

void Texture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(slot, 1, &pTextureView);
}