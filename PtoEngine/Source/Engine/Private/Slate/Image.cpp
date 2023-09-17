
#include "Slate/Image.h"

#include <wincodec.h>

#include "Helper/RectHelper.h"

S_Image::S_Image(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos, std::wstring inFileName)
	: SlateSlotBase(inD2DRT, inSize, inSlateInfos)
{
	SetFileName(inFileName);
}
S_Image::S_Image(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, std::wstring inFileName)	
	: S_Image(inD2DRT, { 0,0 }, inSlateInfos, inFileName)
{}
S_Image::~S_Image()
{
	Util::SafeRelease(pBitmap);
}

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------
void S_Image::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	if (pBitmap != nullptr)
	{
		pD2DRT->DrawBitmap(
			pBitmap,
			RectHelper::ConvertRectToD2D(GetRect()),
			1.f,
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			nullptr
		);
	}
#if _DEBUG
	pD2DRT->DrawRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
#endif
}

void S_Image::SetFileName(std::wstring in)
{
	if (in.empty())
	{
		return;
	}
	if (mFileName == in)
	{
		return;
	}
	mFileName = in;

	if (pBitmap)
	{
		pBitmap->Release();
		pBitmap = nullptr;
	}

	IWICImagingFactory* pImageFactory;
	IWICBitmapDecoder* pBitmapDecoder;
	IWICBitmapFrameDecode* pBitmapFrameDecode;
	IWICFormatConverter* pImageConverter;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pImageFactory);
	pImageFactory->CreateDecoderFromFilename(in.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
	pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	pImageFactory->CreateFormatConverter(&pImageConverter);
	pImageConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 1.f, WICBitmapPaletteTypeMedianCut);

	pD2DRT->CreateBitmapFromWicBitmap(pImageConverter, nullptr, &pBitmap);

	pImageFactory->Release();
	pBitmapDecoder->Release();
	pBitmapFrameDecode->Release();
	pImageConverter->Release();
}