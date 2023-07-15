
#include "Slate/Image.h"

#include <wincodec.h>

S_Image::S_Image(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, std::wstring inFileName)
	: SlateSlotBase(dx, inSize, inD2DRT, inSlateInfos), m_FileName(inFileName)
{
	SetFileName(m_FileName);
}

void S_Image::Draw()
{
	if (m_pBitmap != nullptr)
	{
		m_pD2DRenderTarget->DrawBitmap(
			m_pBitmap,
			GetRect(),
			1.f,
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			nullptr
		);
	}

	m_pD2DRenderTarget->DrawRectangle(GetRect(), m_pBrush);
}

void S_Image::SetFileName(std::wstring in)
{
	if (in.empty())
	{
		return;
	}
	if (m_FileName == in)
	{
		return;
	}
	m_FileName = in;

	IWICImagingFactory* pImageFactory;
	IWICBitmapDecoder* pBitmapDecoder;
	IWICBitmapFrameDecode* pBitmapFrameDecode;
	IWICFormatConverter* pImageConverter;
	CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pImageFactory
	);
	pImageFactory->CreateDecoderFromFilename(
		in.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pBitmapDecoder
	);
	pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	pImageFactory->CreateFormatConverter(&pImageConverter);
	pImageConverter->Initialize(
		pBitmapFrameDecode,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.f,
		WICBitmapPaletteTypeMedianCut
	);

	m_pD2DRenderTarget->CreateBitmapFromWicBitmap(pImageConverter, nullptr, &m_pBitmap);
}