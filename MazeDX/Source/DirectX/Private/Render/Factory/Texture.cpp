
#include "Render/Factory/Texture.h"

#include "Render/Manager/BindableManager.h"

#include "DirectX/WICTextureLoader/WICTextureLoader11.h"

Texture::Texture(DirectX11& dx, const std::wstring& inFileName, UINT inSlot)
	: fileName(inFileName), slot(inSlot)
{
	HRESULT result = DirectX::LoadFromWICFile(
		inFileName.c_str(),
		DirectX::WIC_FLAGS_IGNORE_SRGB,
		nullptr,
		scratch
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not load Texture", L"Failed Texture constructor", MB_OK);
		assert(false);
	}

	if (scratch.GetImage(0, 0, 0)->format != format)
	{
		DirectX::ScratchImage converted;
		result = DirectX::Convert(
			*scratch.GetImage(0, 0, 0),
			format,
			DirectX::TEX_FILTER_DEFAULT,
			0.5,
			converted
		);
		if (FAILED(result))
		{
			MessageBox(NULL, L"Can not load Convert Texture", L"Failed Texture constructor", MB_OK);
			assert(false);
		}
		scratch = std::move(converted);
	}

	UINT width = (UINT)scratch.GetImage(0, 0, 0)->width;
	UINT height = (UINT)scratch.GetImage(0, 0, 0)->height;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	
	ID3D11Texture2D* pTexture;
	GetDevice(dx)->CreateTexture2D(&texDesc, nullptr, &pTexture);

	GetContext(dx)->UpdateSubresource(
		pTexture,
		0,
		nullptr,
		scratch.GetPixels(),
		width * sizeof(DWORD),
		0
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GetDevice(dx)->CreateShaderResourceView(pTexture, &srvDesc, &m_pTextureView);

	GetContext(dx)->GenerateMips(m_pTextureView);
}
Texture::~Texture()
{
	//Util::SafeRelease(m_pTextureView);
	//fileName.empty();
}

std::shared_ptr<Texture> Texture::Make(DirectX11& dx, const std::wstring& inFileName, UINT inSlot)
{
	return BindableManager::Make<Texture>(dx, inFileName, inSlot);
}

void Texture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(slot, 1, &m_pTextureView);
}