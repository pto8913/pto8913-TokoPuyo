
#include "Render/ScreenText.h"

#include "Render/BindableManager.h"
#include <dwrite.h>

ScreenText::ScreenText(DirectX11& dx, float inWidth, float inHeight)
	: width(inWidth), height(inHeight)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = (UINT)width;
	texDesc.Height = (UINT)height;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	ID3D11Texture2D* sharedTex11;
	HRESULT result = dx.GetDevice()->CreateTexture2D(&texDesc, NULL, &sharedTex11);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ScreenText", L"Failed ScreenText constructor", MB_OK);
		assert(false);
	}

	// Get the keyed mutex for the shared texture (for D3D11)
	sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);

	// Get the shared handle needed to open the shared texture in D3D10.1
	IDXGIResource* sharedResource10;
	HANDLE sharedHandle10;

	sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);

	sharedResource10->GetSharedHandle(&sharedHandle10);

	sharedResource10->Release();

	// Open the surface for the shared texture in D3D10.1
	IDXGISurface1* sharedSurface10;

	dx.GetDevice()->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));

	sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);

	// Create D2D factory
	ID2D1Factory* D2DFactory;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

	sharedSurface10->Release();
	D2DFactory->Release();

	// Create a solid color brush to draw something with		
	D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 255.0f, 1.0f), &pBrush);

	//DirectWrite
	IDWriteFactory* pDWriteFactory = nullptr;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

	pDWriteFactory->CreateTextFormat(
		L"Script",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&TextFormat
	);
	pDWriteFactory->Release();

	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	dx.GetDevice()->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
}
ScreenText::~ScreenText()
{
	Util::SafeRelease(keyedMutex11);
	Util::SafeRelease(keyedMutex10);
	Util::SafeRelease(D2DRenderTarget);
	Util::SafeRelease(pBrush);
	Util::SafeRelease(d2dTexture);
	Util::SafeRelease(TextFormat);
}

std::shared_ptr<ScreenText> ScreenText::Make(DirectX11& dx, float inWidth, float inHeight)
{
	return BindableManager::Make<ScreenText>(dx, inWidth, inHeight);
}

void ScreenText::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(0, 1, &d2dTexture);
}
void ScreenText::Bind(std::wstring text, D2D1_RECT_F rect)
{
	//Release the D3D 11 Device
	keyedMutex11->ReleaseSync(0);

	//Use D3D10.1 device
	keyedMutex10->AcquireSync(0, 5);

	//Draw D2D content		
	D2DRenderTarget->BeginDraw();

	//Clear D2D Background
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	//Create our string
	std::wostringstream printString;
	printString << text;
	printText = printString.str();

	//Set the Font Color
	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	//Set the brush color D2D will use to draw with
	pBrush->SetColor(FontColor);

	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, (FLOAT)width, (FLOAT)height);

	//Draw the Text
	D2DRenderTarget->DrawText(
		printText.c_str(),
		(UINT)wcslen(printText.c_str()),
		TextFormat,
		layoutRect,
		pBrush
	);

	//D2D1_RECT_F ll = D2D1::RectF(
	//	Config::GAMEUI_LEFT_TOP.x, 
	//	Config::GAMEUI_LEFT_TOP.y,
	//	Config::GAMEUI_RIGHT_BOTTOM.x,
	//	Config::GAMEUI_LEFT_TOP.y + 200
	//);

	//D2DRenderTarget->DrawRectangle(ll, pBrush, 2);
	D2DRenderTarget->FillRectangle(rect, pBrush);

	D2DRenderTarget->EndDraw();

	//Release the D3D10.1 Device
	keyedMutex10->ReleaseSync(1);

	//Use the D3D11 Device
	keyedMutex11->AcquireSync(1, 5);
}

ScreenTextOnlyOutput::ScreenTextOnlyOutput(DirectX11& dx, float inWidth, float inHeight)
	: width(inWidth), height(inHeight)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = (UINT)width;
	texDesc.Height = (UINT)height;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	ID3D11Texture2D* sharedTex11;
	HRESULT result = dx.GetDevice()->CreateTexture2D(&texDesc, NULL, &sharedTex11);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create ScreenText", L"Failed ScreenText constructor", MB_OK);
		assert(false);
	}

	// Get the keyed mutex for the shared texture (for D3D11)
	sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);

	// Get the shared handle needed to open the shared texture in D3D10.1
	IDXGIResource* sharedResource10;
	HANDLE sharedHandle10;

	sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);

	sharedResource10->GetSharedHandle(&sharedHandle10);

	sharedResource10->Release();

	// Open the surface for the shared texture in D3D10.1
	IDXGISurface1* sharedSurface10;

	dx.GetDevice()->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));

	sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);

	// Create D2D factory
	ID2D1Factory* D2DFactory;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

	sharedSurface10->Release();
	D2DFactory->Release();

	dx.GetDevice()->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
}
ScreenTextOnlyOutput::~ScreenTextOnlyOutput()
{
	Util::SafeRelease(d2dTexture);
	Util::SafeRelease(D2DRenderTarget);
	Util::SafeRelease(keyedMutex11);
	Util::SafeRelease(keyedMutex10);
}

std::shared_ptr<ScreenTextOnlyOutput> ScreenTextOnlyOutput::Make(DirectX11& dx, float inWidth, float inHeight)
{
	return BindableManager::Make<ScreenTextOnlyOutput>(dx, inWidth, inHeight);
}

void ScreenTextOnlyOutput::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(0, 1, &d2dTexture);
}
