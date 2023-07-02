
#include "Render/Factory/Texture.h"

#include "Render/Manager/BindableManager.h"

#include "DirectX/WICTextureLoader/WICTextureLoader11.h"

Texture::Texture(DirectX11& dx, const std::wstring& inFileName, UINT inSlot)
	: fileName(inFileName), slot(inSlot)
{
	HRESULT result = DirectX::CreateWICTextureFromFile(
		GetDevice(dx), 
		inFileName.c_str(), 
		(ID3D11Resource**)&m_pTexture,
		&m_pTextureView
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not load Texture", L"Failed Texture constructor", MB_OK);
		assert(false);
	}
}
Texture::~Texture()
{
	m_pTexture->Release();
	m_pTextureView->Release();
}

std::shared_ptr<Texture> Texture::Make(DirectX11& dx, const std::wstring& inFileName, UINT inSlot)
{
	return BindableManager::Make<Texture>(dx, inFileName, inSlot);
}

void Texture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(slot, 1, &m_pTextureView);
}