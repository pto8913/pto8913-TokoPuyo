

#include "Slate/TextBlock.h"
#include "Slate/CustomTextRenderer.h"

#include "Core/DirectX.h"

#include "Utils/String.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

#include "EngineSettings.h"

#include <wincodec.h>

S_TextBlock::S_TextBlock(ID2D1RenderTarget* inRt2D, DirectX11& dx, FVector2D inSize, FSlateInfos inSlateInfos, FSlateFont inFont, FSlateTextAppearance inAppearance)
	: SlateSlotBase(inRt2D, inSize, inSlateInfos), mText(L""), mFont(inFont), mAppearance(inAppearance)
{
	CreateDeviceResource(dx);

	pRt2D->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.color),
		&pBrush
	);
	mTextLength = (UINT32)mText.length();
	UpdateTextLayout();

	SetFont(mFont);
	SetAppearance(mAppearance);
}
S_TextBlock::S_TextBlock(ID2D1RenderTarget* inRt2D, DirectX11& dx, FSlateInfos inSlateInfos, FSlateFont inFont, FSlateTextAppearance inAppearance)
	: S_TextBlock(inRt2D, dx, { 0,0 }, inSlateInfos, inFont, inAppearance)
{
}

S_TextBlock::~S_TextBlock()
{
	Util::SafeRelease(pDWriteFactory);
	Util::SafeRelease(pTextFormat);
	Util::SafeRelease(pTextLayout);
	OnSetText.ClearBind();
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void S_TextBlock::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	/* For WidgetAnimation */
	pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.color));
#if _DEBUG
	pRt2D->DrawRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
#endif

	if (mAppearance.layoutOutline)
	{
		pTextLayout->Draw(NULL, pCustomTextRenderer, mPosition.x, mPosition.y);
		return;
	}
	pRt2D->DrawText(
		mText.c_str(),
		(UINT32)mText.size(),
		pTextFormat,
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
}

void S_TextBlock::SetAppearance(FSlateTextAppearance in)
{
	mAppearance = in;

	SetAppearHorizontalAlignment(mAppearance.hAlign);
	SetAppearVerticalAlignment(mAppearance.vAlign);
	SetWrap(mAppearance.wrap);

	pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.color));

	UpdateTextLayout();
}
FSlateTextAppearance& S_TextBlock::GetAppearance()
{
	return mAppearance;
}

void S_TextBlock::SetFont(FSlateFont inFont)
{
	mFont = inFont;

	pDWriteFactory->CreateTextFormat(
		mFont.fontFamily.c_str(),
		NULL,
		mFont.weight,
		mFont.style,
		mFont.stretch,
		mFont.fontSize,
		mFont.fontLocalName.c_str(),
		&pTextFormat
	);
	UpdateSize();
	UpdateTextLayout();
}
void S_TextBlock::SetText(std::wstring inText)
{
	mText = inText;
	mTextLength = (UINT32)mText.length();

	UpdateSize();
	UpdateTextLayout();
}
void S_TextBlock::SetSize(FVector2D inSize)
{
	mSize.x = inSize.x;
	//mSize.x = mFont.fontSize * mText.size();
	//mSize.x = inSize.x;
	//mSize.y = mFont.fontSize;
	mSize.y = inSize.y;
}
void S_TextBlock::UpdateSize()
{
	auto lines = Util::Split(mText, L'\n');
	UINT8 lineCount = (UINT8)lines.size();
	if (lineCount > 1)
	{
		SetSize(
			{
				mFont.fontSize * lines[0].size(),
				mFont.fontSize * lines.size()
			}
		);
	}
	else
	{
		SetSize(
			{
				max(mSize.x, mFont.fontSize * mText.size()),
				max(mSize.y, mFont.fontSize)
			}
		);
		//SetSize(
		//	{
		//		mFont.fontSize * mText.size(),
		//		mSize.y
		//	}
		//);
	}
}

void S_TextBlock::SetAppearHorizontalAlignment(EHorizontalAlignment in)
{
	mAppearance.hAlign = in;
	switch (in)
	{
	case EHorizontalAlignment::Left:
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case EHorizontalAlignment::Right:
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	default:
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	}
}
void S_TextBlock::SetAppearVerticalAlignment(EVerticalAlignment in)
{
	mAppearance.vAlign = in;
	switch (in)
	{
	case EVerticalAlignment::Top:
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case EVerticalAlignment::Bottom:
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	default:
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	}
}
void S_TextBlock::SetWrap(ETextWrap in)
{
	mAppearance.wrap = in;
	switch (in)
	{
	case ETextWrap::No:
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		break;
	case ETextWrap::Word:
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WHOLE_WORD);
		break;
	case ETextWrap::Character:
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
		break;
	case ETextWrap::BreakWord:
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK);
		break;
	default:
		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		break;
	}
}

void S_TextBlock::CreateDeviceResource(DirectX11& dx)
{
	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void**>(&pWICFactory)
	);
	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		&pD2DFactory
	);
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(IDWriteFactory), 
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
	);

	UpdateTextLayout();
	
	ID2D1Bitmap* pBitmap = nullptr;
	LoadResourceBitmap(pRt2D, pWICFactory, L"Tulip", L"Image", &pBitmap);

	if (pBitmapBrush == nullptr)
	{
		// Create the bitmap brush
		D2D1_BITMAP_BRUSH_PROPERTIES properties = { D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR };
		pRt2D->CreateBitmapBrush(
			pBitmap,
			properties,
			&pBitmapBrush
		);
	}
	if (pCustomTextRenderer == nullptr)
	{
		pCustomTextRenderer = new CustomTextRenderer(
			pD2DFactory,
			pRt2D,
			pBrush,
			pBitmapBrush
		);
	}
	Util::SafeRelease(pBitmap);
}

void S_TextBlock::UpdateTextLayout()
{
	if (pTextLayout)
	{
		pTextLayout->Release();
		pTextLayout = nullptr;
	}
	pDWriteFactory->CreateTextLayout(
		mText.c_str(),
		mTextLength,
		pTextFormat,
		mSize.x / 96.f,
		mSize.y / 96.f,
		&pTextLayout
	);
	if (pTextLayout)
	{
		DWRITE_TEXT_RANGE textRange;
		textRange.length = mTextLength;
		pTextLayout->SetFontSize(mFont.fontSize, textRange);
		pTextLayout->SetUnderline(mAppearance.layoutUnderLine, textRange);
		pTextLayout->SetFontWeight(mAppearance.layoutWeight, textRange);
		IDWriteTypography* pTypography = nullptr;
		pDWriteFactory->CreateTypography(&pTypography);
		if (pTypography != nullptr)
		{
			pTypography->AddFontFeature(mAppearance.layoutFeature);
			pTextLayout->SetTypography(pTypography, textRange);
		}
		Util::SafeRelease(pTypography);
	}
}


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

HRESULT S_TextBlock::LoadResourceBitmap(
	ID2D1RenderTarget* pRT,
	IWICImagingFactory* pIWICFactory,
	PCWSTR resourceName,
	PCWSTR resourceType,
	__deref_out ID2D1Bitmap** ppBitmap
)
{
	HRESULT hr = S_OK;

	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	// Locate the resource handle in our dll
	imageResHandle = FindResourceW(
		HINST_THISCOMPONENT,
		resourceName,
		resourceType
	);

	hr = imageResHandle ? S_OK : E_FAIL;

	// Load the resource
	imageResDataHandle = LoadResource(
		HINST_THISCOMPONENT,
		imageResHandle
	);

	if (SUCCEEDED(hr))
	{
		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	// Lock it to get a system memory pointer
	pImageFile = LockResource(
		imageResDataHandle
	);

	if (SUCCEEDED(hr))
	{
		hr = pImageFile ? S_OK : E_FAIL;
	}

	// Calculate the size
	imageFileSize = SizeofResource(
		HINST_THISCOMPONENT,
		imageResHandle
	);

	if (SUCCEEDED(hr))
	{
		hr = imageFileSize ? S_OK : E_FAIL;
	}

	// Create a WIC stream to map onto the memory
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateStream(&pStream);
	}

	// Initialize the stream with the memory pointer and size
	if (SUCCEEDED(hr))
	{
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	// Create a decoder for the stream
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	// Create the initial frame
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(
			0,
			&pSource
		);
	}

	// Format convert to 32bppPBGRA -- which Direct2D expects
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	// Create a Direct2D bitmap from the WIC bitmap.
	if (SUCCEEDED(hr))
	{
		hr = pRT->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	Util::SafeRelease(pDecoder);
	Util::SafeRelease(pSource);
	Util::SafeRelease(pStream);
	Util::SafeRelease(pConverter);

	return hr;
}
