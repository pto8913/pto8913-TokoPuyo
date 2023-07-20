
#include "Render/Factory/Texture.h"

#include "Render/Manager/BindableManager.h"

#include "DXTex/WICTextureLoader11.h"
#include "DXTex/DDSTextureLoader11.h"

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
			(ID3D11Resource**)&m_pTexture,
			&m_pTextureView
		);
		break;
	default:
		result = DirectX::CreateWICTextureFromFile(
			GetDevice(dx),
			inFileName.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pTextureView
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
	Util::SafeRelease(m_pTexture);
	Util::SafeRelease(m_pTextureView);
}

std::shared_ptr<Texture> Texture::Make(DirectX11& dx, Texture::TextureType inTextureType, const std::wstring& inFileName, UINT inSlot)
{
	return BindableManager::Make<Texture>(dx, inTextureType, inFileName, inSlot);
}

void Texture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(slot, 1, &m_pTextureView);
}