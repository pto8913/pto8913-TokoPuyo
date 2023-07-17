
#include "Render/Factory/ScreenTexture.h"

#include <D3D10_1.h>
#include <d3d10.h>

#include "Render/Manager/BindableManager.h"

#include <wincodec.h>

#include "SDK/DXTex/include/WICTextureLoader/WICTextureLoader11.h"

ScreenTexture::ScreenTexture(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight)
	: fileName(inFileName), width(inWidth), height(inHeight)
{
	ID3D11Texture2D* srcTexture;
	HRESULT result = DirectX::CreateWICTextureFromFileEx(
		dx.GetDevice(),
		inFileName.c_str(),
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		D3D11_CPU_ACCESS_READ,
		D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX,
		DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
		(ID3D11Resource**)&srcTexture,
		&d2dTexture
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ScreenTexture", L"Failed ScreenTexture constructor", MB_OK);
		assert(false);
	}

	IWICImagingFactory* pImageFactory;
	IWICBitmapDecoder* pBitmapDecoder;
	IWICBitmapFrameDecode* pBitmapFrameDecode;
	IWICFormatConverter* pImageConverter;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pImageFactory);
	pImageFactory->CreateDecoderFromFilename(inFileName.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
	pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	pImageFactory->CreateFormatConverter(&pImageConverter);
	pImageConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 1.f, WICBitmapPaletteTypeMedianCut);


	D3D11_TEXTURE2D_DESC texDesc;
	srcTexture->GetDesc(&texDesc);
	texDesc.Usage = D3D11_USAGE_STAGING;
	texDesc.BindFlags = 0;
	texDesc.MiscFlags = 0;
	ID3D11Texture2D* stagTexture;
	result = dx.GetDevice()->CreateTexture2D(&texDesc, NULL, &stagTexture);
	dx.GetContext()->CopyResource(stagTexture, srcTexture);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create Copied ScreenTexture", L"Failed ScreenTexture constructor", MB_OK);
		assert(false);
	}

	// Get the keyed mutex for the shared texture (for D3D11)
	result = srcTexture->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create keyedMutex11", L"Failed ScreenTexture constructor", MB_OK);
		assert(false);
	}
	// Get the shared handle needed to open the shared texture in D3D10.1
	IDXGIResource* sharedResource10;
	HANDLE sharedHandle10;

	srcTexture->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);
	sharedResource10->GetSharedHandle(&sharedHandle10);
	// Open the surface for the shared texture in D3D10.1
	IDXGISurface1* sharedSurface10;
	dx.GetDevice()->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));
	sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);

	IDXGISurface* texStagDXGI;
	stagTexture->QueryInterface(__uuidof(IDXGISurface), (void**)&texStagDXGI);
	
	ID2D1Factory* D2DFactory;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	IDXGISurface* surface;
	dx.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	result = D2DFactory->CreateDxgiSurfaceRenderTarget(surface, &renderTargetProperties, &D2DRenderTarget);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create D2DRenderTarget", L"Failed ScreenTexture constructor", MB_OK);
		assert(false);
	}

	//DXGI_MAPPED_RECT bitmap2Dmap;
	//texStagDXGI->Map(&bitmap2Dmap, DXGI_MAP_READ);

	//D2D1_SIZE_U size = D2D1::SizeU((UINT32)inWidth, (UINT32)inHeight);

	//D2D1_BITMAP_PROPERTIES bitmapProps;
	//bitmapProps.dpiX = inWidth;
	//bitmapProps.dpiY = inHeight;
	//bitmapProps.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//bitmapProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	//D2DRenderTarget->CreateBitmap(
	//	size, (void*)bitmap2Dmap.pBits, bitmap2Dmap.Pitch, bitmapProps, &m_pBitmap
	//);
	//texStagDXGI->Unmap();
	D2DRenderTarget->CreateBitmapFromWicBitmap(pImageConverter, nullptr, &m_pBitmap);
}
ScreenTexture::~ScreenTexture()
{
	D2DRenderTarget->Release();
	d2dTexture->Release();
}

std::shared_ptr<ScreenTexture> ScreenTexture::Make(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight)
{
	return BindableManager::Make<ScreenTexture>(dx, inFileName, inWidth, inHeight);
}

void ScreenTexture::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(0, 1, &d2dTexture);
}

void ScreenTexture::Bind(DirectX::XMFLOAT2 offset)
{
	////Release the D3D 11 Device
	//keyedMutex11->ReleaseSync(0);

	////Use D3D10.1 device
	//keyedMutex10->AcquireSync(0, 5);

	//Draw D2D content		
	D2DRenderTarget->BeginDraw();

	//Clear D2D Background
	//D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(offset.x, offset.y, (FLOAT)width + offset.x, (FLOAT)height + offset.y);
	
	D2DRenderTarget->DrawBitmap(
		m_pBitmap, 
		layoutRect, 
		1.f, 
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, 
		nullptr
	);

	D2DRenderTarget->EndDraw();

	////Release the D3D10.1 Device
	//keyedMutex10->ReleaseSync(1);

	////Use the D3D11 Device
	//keyedMutex11->AcquireSync(1, 5);
}