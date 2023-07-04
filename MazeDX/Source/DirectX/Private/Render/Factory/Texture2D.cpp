
#include "Render/Factory/Texture2D.h"

#include "Render/Manager/BindableManager.h"

Texture2D::Texture2D(DirectX11& dx, UINT inWidth, UINT inHeight)
	: width(inWidth), height(inHeight)
{
	//Create Shared Texture that Direct3D 10.1 will render on
	D3D11_TEXTURE2D_DESC sharedTexDesc;
	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));
	sharedTexDesc.Width            = width;
	sharedTexDesc.Height           = height;
	sharedTexDesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels        = 1;
	sharedTexDesc.ArraySize        = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.SampleDesc.Quality = 0;
	sharedTexDesc.Usage            = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sharedTexDesc.MiscFlags        = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
	sharedTexDesc.CPUAccessFlags = 0;
	ID3D11Texture2D* sharedTex11;
	HRESULT result = dx.GetDevice()->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create Texture2D", L"Failed Texture2D constructor", MB_OK);
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
	D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 255.0f, 1.0f), &Brush);

	//DirectWrite
	IDWriteFactory* DWriteFactory;
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&DWriteFactory)
	);

	DWriteFactory->CreateTextFormat(
		L"Script",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&TextFormat
	);

	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	//Create A shader resource view from the texture D2D will render to,
	//So we can use it to texture a square which overlays our scene
	dx.GetDevice()->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
}
Texture2D::~Texture2D()
{
	keyedMutex11->Release();
	keyedMutex10->Release();
	D2DRenderTarget->Release();
	Brush->Release();
	d2dTexture->Release();
	TextFormat->Release();
}

std::shared_ptr<Texture2D> Texture2D::Make(DirectX11& dx, UINT inWidth, UINT inHeight)
{
	return BindableManager::Make<Texture2D>(dx, inWidth, inHeight);
}

void Texture2D::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShaderResources(0, 1, &d2dTexture);
}
void Texture2D::Bind(std::wstring text)
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
	Brush->SetColor(FontColor);

	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, (FLOAT)width, (FLOAT)height);

	//Draw the Text
	D2DRenderTarget->DrawText(
		printText.c_str(),
		(UINT)wcslen(printText.c_str()),
		TextFormat,
		layoutRect,
		Brush
	);

	D2DRenderTarget->EndDraw();

	//Release the D3D10.1 Device
	keyedMutex10->ReleaseSync(1);

	//Use the D3D11 Device
	keyedMutex11->AcquireSync(1, 5);
}