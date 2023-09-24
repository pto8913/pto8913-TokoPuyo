
#include "Graphics/Render/BindableManager.h"

#include "Graphics/Render/CubeTexture.h"
#include "Graphics/Render/RenderTargetView.h"
#include "Graphics/Render/DepthStencilView.h"

#include "DDSTextureLoader11.h"
#include <vector>

CubeTexture::CubeTexture(DirectX11& dx, const std::wstring& path, UINT newSlot)
	: slot(newSlot), fileName(path)
{
	ID3D11Texture2D* pTexture = 0;
	DirectX::CreateDDSTextureFromFile(
		GetDevice(dx),
		path.c_str(),
		(ID3D11Resource**)&pTexture,
		0
	);

	//Create the textures description
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture->GetDesc(&texDesc);

	//Tell D3D We have a cube texture, which is an array of 2D textures
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = texDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	//Create the Resource view
	GetDevice(dx)->CreateShaderResourceView(pTexture, &SMViewDesc, &pTextureView);
}
CubeTexture::~CubeTexture()
{
	Util::SafeRelease(pTextureView);
}

std::shared_ptr<CubeTexture> CubeTexture::Make(DirectX11& dx, const std::wstring& inFileName, UINT inSlot)
{
	return BindableManager::Make<CubeTexture>(dx, inFileName, inSlot);
}
void CubeTexture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(slot, 1, &pTextureView);
}