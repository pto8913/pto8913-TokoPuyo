
#include "Graphics/Render/ScreenTexture.h"

#include <D3D10_1.h>
#include <d3d10.h>

#include "Graphics/Render/BindableManager.h"

#include <wincodec.h>

#include "WICTextureLoader11.h"

IWICImagingFactory* pImageFactory;

ScreenTexture::ScreenTexture(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight)
	: fileName(inFileName), width(inWidth), height(inHeight)
{
	ID2D1Factory* D2DFactory = nullptr;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	IDXGISurface* surface = nullptr;
	dx.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	HRESULT result = D2DFactory->CreateDxgiSurfaceRenderTarget(surface, &renderTargetProperties, &D2DRenderTarget);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create D2DRenderTarget", L"Failed ScreenTexture constructor", MB_OK);
		assert(false);
	}

	UpdateTexture(inFileName);
}
ScreenTexture::~ScreenTexture()
{
	//Util::SafeRelease(D2DRenderTarget);
	//Util::SafeRelease(pBitmap);
}

std::shared_ptr<ScreenTexture> ScreenTexture::Make(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight)
{
	return BindableManager::Make<ScreenTexture>(dx, inFileName, inWidth, inHeight);
}

void ScreenTexture::UpdateTexture(const std::wstring& inFileName)
{
	if (pBitmap)
	{
		pBitmap->Release();
		pBitmap = nullptr;
	}

	IWICBitmapDecoder* pBitmapDecoder;
	IWICBitmapFrameDecode* pBitmapFrameDecode;
	IWICFormatConverter* pImageConverter;
	if (pImageFactory == nullptr)
	{
		HRESULT result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pImageFactory);
	}
	pImageFactory->CreateDecoderFromFilename(inFileName.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
	pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	pImageFactory->CreateFormatConverter(&pImageConverter);
	pImageConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 1.f, WICBitmapPaletteTypeMedianCut);

	HRESULT result = D2DRenderTarget->CreateBitmapFromWicBitmap(pImageConverter, nullptr, &pBitmap);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create Bitmap", L"Failed ScreenTexture constructor", MB_OK);
		assert(false);
	}

	pBitmapDecoder->Release();
	pBitmapFrameDecode->Release();
	pImageConverter->Release();
}
void ScreenTexture::UpdateWidth(const float& inWidth)
{
	width = inWidth;
}
void ScreenTexture::UpdateHeight(const float& inHeight)
{
	height = inHeight;
}
void ScreenTexture::UpdateSize(const float& inWidth, const float& inHeight)
{
	width = inWidth;
	height = inHeight;
}

void ScreenTexture::Bind(DirectX11& dx)
{
	//GetContext(dx)->PSSetShaderResources(0, 1, &d2dTexture);
}
void ScreenTexture::Bind(const DirectX::XMVECTOR& loc, const float& angle)
{
	D2DRenderTarget->BeginDraw();

	//Create the D2D Render Area
	float x = DirectX::XMVectorGetX(loc);
	float y = DirectX::XMVectorGetY(loc);
	D2D1_RECT_F layoutRect = D2D1::RectF(x, y, width + x, height + y);

	D2DRenderTarget->DrawBitmap(
		pBitmap,
		layoutRect,
		1.f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		nullptr
	);
	if (angle != 0.f)
	{
		D2DRenderTarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(
				angle,
				D2D1::Point2F(
					x,// + width / 2.f,
					y// + height / 2.f
				)
			)
		);
	}
	D2DRenderTarget->EndDraw();
}

ID2D1RenderTarget* ScreenTexture::GetRt2D()
{
	return D2DRenderTarget;
}