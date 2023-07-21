
#include "Render/Manager/BindableManager.h"

#include "Render/Factory/CubeTexture.h"
#include "Render/Factory/RenderTargetView.h"
#include "Render/Factory/DepthStencilView.h"

#include "SDK/DXTex/include/Public/DDSTextureLoader11.h"
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
	GetDevice(dx)->CreateShaderResourceView(pTexture, &SMViewDesc, &m_pTextureView);
	//std::vector<DirectX::ScratchImage> surfaces(6);

	//for (int i = 0; i < 6; ++i)
	//{
	//	DirectX::ScratchImage& scratch = surfaces[i];
	//	std::string filename = path + std::string("/") + std::to_string(i) + std::string(".png");
	//	std::wstring wfilename = Util::s2WString(filename);
	//	
	//	DirectX::LoadFromWICFile(
	//		wfilename.c_str(),
	//		DirectX::WIC_FLAGS_IGNORE_SRGB,
	//		nullptr,
	//		scratch
	//	);
	//	if (scratch.GetImage(0, 0, 0)->format != DXGI_FORMAT_B8G8R8A8_UNORM)
	//	{
	//		DirectX::ScratchImage converted;
	//		DirectX::Convert(
	//			*surfaces[i].GetImage(0, 0, 0),
	//			DXGI_FORMAT_B8G8R8A8_UNORM,
	//			DirectX::TEX_FILTER_DEFAULT,
	//			DirectX::TEX_THRESHOLD_DEFAULT,
	//			converted
	//		);
	//	}
	//}

	//D3D11_TEXTURE2D_DESC textureDesc;
	//textureDesc.Width = (UINT)surfaces[0].GetMetadata().width;
	//textureDesc.Height = (UINT)surfaces[0].GetMetadata().height;
	//textureDesc.MipLevels = 1;
	//textureDesc.ArraySize = 6;
	//textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//textureDesc.SampleDesc.Count = 1;
	//textureDesc.SampleDesc.Quality = 0;
	//textureDesc.Usage = D3D11_USAGE_DEFAULT;
	//textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//textureDesc.CPUAccessFlags = 0;
	//textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//D3D11_SUBRESOURCE_DATA data[6];
	//for (int i = 0; i < 6; ++i)
	//{
	//	data[i].pSysMem = surfaces[i].GetPixels();
	//	data[i].SysMemPitch = (UINT)surfaces[i].GetImage(0, 0, 0)->rowPitch;
	//	data[i].SysMemSlicePitch = 0;
	//}
	//ID3D11Texture2D* pTexture;
	//HRESULT result = GetDevice(dx)->CreateTexture2D(&textureDesc, data, &pTexture);
	//if (FAILED(result))
	//{
	//	MessageBox(NULL, L"Can not create Texture", L"Failed Texture", MB_OK);
	//	assert(false);
	//}
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//srvDesc.Format = textureDesc.Format;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.Texture2D.MipLevels = 1;
	//GetDevice(dx)->CreateShaderResourceView(
	//	pTexture, 
	//	&srvDesc, 
	//	&m_pTextureView
	//);
}
CubeTexture::~CubeTexture()
{
	Util::SafeRelease(m_pTextureView);
}

std::shared_ptr<CubeTexture> CubeTexture::Make(DirectX11& dx, const std::wstring& inFileName, UINT inSlot)
{
	return BindableManager::Make<CubeTexture>(dx, inFileName, inSlot);
}
void CubeTexture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(slot, 1, &m_pTextureView);
}
//
//CubeTargetTexture::CubeTargetTexture(DirectX11& dx, UINT width, UINT height, UINT inSlot, DXGI_FORMAT format)
//	: slot(inSlot)
//{
//	D3D11_TEXTURE2D_DESC textureDesc;
//	textureDesc.Width = width;
//	textureDesc.Height = height;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 6;
//	textureDesc.Format = format;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.SampleDesc.Quality = 0;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//
//	ID3D11Texture2D* pTexture;
//	GetDevice(dx)->CreateTexture2D(&textureDesc, nullptr, &pTexture);
//
//	for (UINT face = 0; face < 6; ++face)
//	{
//		renderTargets.push_back(std::make_shared<RenderTargetView>(dx, pTexture));
//	}
//}
//std::shared_ptr<RenderTargetView> CubeTargetTexture::Get(size_t index) const
//{
//	return renderTargets[index];
//}
//
//CubeDepthTexture::CubeDepthTexture(DirectX11& dx, UINT size, UINT inSlot)
//	: slot(inSlot)
//{
//	// texture descriptor
//	D3D11_TEXTURE2D_DESC textureDesc = {};
//	textureDesc.Width = size;
//	textureDesc.Height = size;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 6;
//	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.SampleDesc.Quality = 0;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//	// create the texture resource
//	ID3D11Texture2D* pTexture;
//	GetDevice(dx)->CreateTexture2D(
//		&textureDesc, nullptr, &pTexture
//	);
//
//	// create the resource view on the texture
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	srvDesc.Texture2D.MipLevels = 1;
//	GetDevice(dx)->CreateShaderResourceView(
//		pTexture, &srvDesc, &m_pTextureView
//	);
//
//	// make depth buffer resources for capturing shadow map
//	for (UINT face = 0; face < 6; face++)
//	{
//		depthBuffers.push_back(
//			std::make_shared<DepthStencilView>(
//				dx, pTexture, face
//			)
//		);
//	}
//}
//std::shared_ptr<DepthStencilView> CubeDepthTexture::Get(size_t index) const
//{
//	return depthBuffers[index];
//}