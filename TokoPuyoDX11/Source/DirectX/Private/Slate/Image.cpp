
#include "Slate/Image.h"

#include <wincodec.h>

S_Image::S_Image(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, std::wstring inFileName)
	: SlateSlotBase(inSize, inD2DRT, inSlateInfos), m_FileName(inFileName)
{
	SetFileName(inFileName);
}
S_Image::~S_Image()
{
	//Util::SafeRelease(m_pBitmap);
	m_pBitmap = nullptr;

	SlateSlotBase::~SlateSlotBase();
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
#if _DEBUG
	m_pD2DRenderTarget->DrawRectangle(GetRect(), m_pBrush);
#endif
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

	if (m_pBitmap)
	{
		m_pBitmap->Release();
		m_pBitmap = nullptr;
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
	
	m_pD2DRenderTarget->CreateBitmapFromWicBitmap(pImageConverter, nullptr, &m_pBitmap);

	pImageFactory->Release();
	pBitmapDecoder->Release();
	pBitmapFrameDecode->Release();
	pImageConverter->Release();
}