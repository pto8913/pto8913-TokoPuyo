
#include "UI/Slate/Border.h"
#include <wincodec.h>

#include "Helper/ColorHelper.h"

#if _DEBUG
#include <format>
#endif

S_Border::S_Border(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateBorderAppearance inAppearance)
	: SlotContainerOnlyOne(inSize, inD2DRT, inSlateInfos)
{
	SetAppearance(inAppearance);
}
S_Border::S_Border(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateBorderAppearance inAppearance)
	: S_Border({ 0,0 }, inD2DRT, inSlateInfos, inAppearance)
{
}

void S_Border::Draw()
{
	if (!bIsVisible)
	{
		return;
	}
	switch (appearance.Type)
	{
	case EBorderType::None:
		SlotContainerOnlyOne::Draw();
		return;
	case EBorderType::Box:
		if (appearance.bIsFill)
		{
			m_pD2DRenderTarget->FillRectangle(
				GetRect(),
				m_pBrush
			);
		}
		else
		{
			m_pD2DRenderTarget->DrawRectangle(
				GetRect(),
				m_pBrush,
				appearance.lineWidth
			);
		}
		break;
	case EBorderType::Border:
		if (appearance.bIsFill)
		{
			m_pD2DRenderTarget->FillRoundedRectangle(
				D2D1::RoundedRect(GetRect(), appearance.roundSize.x, appearance.roundSize.y),
				m_pBrush
			);
		}
		else
		{
			m_pD2DRenderTarget->DrawRoundedRectangle(
				D2D1::RoundedRect(GetRect(), appearance.roundSize.x, appearance.roundSize.y),
				m_pBrush,
				appearance.lineWidth
			);
		}
		break;
	case EBorderType::Image:
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
		break;
	default:
		break;
	}

	SlotContainerOnlyOne::Draw();
}

void S_Border::SetAppearance(const FSlateBorderAppearance& inAppearance)
{
	if (inAppearance.Type == EBorderType::Image)
	{
		SetFileName(inAppearance.fileName);
	}
	appearance = inAppearance;

	m_pBrush->SetColor(ColorHelper::ConvertColorToD2D(appearance.color));
}
FSlateBorderAppearance& S_Border::GetAppearance()
{
	return appearance;
}

void S_Border::SetFileName(std::wstring in)
{
	if (in.empty())
	{
		return;
	}
	if (appearance.fileName == in)
	{
		return;
	}
	appearance.fileName = in;

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

void S_Border::Update()
{
	if (GetChildrenCount() == 0) return;

	const float cellH = GetHeight();
	const float cellW = GetWidth();

	DirectX::XMFLOAT2 NewSize = { 0, 0 };
	DirectX::XMFLOAT2 NewPos = { 0, 0 };

	DirectX::XMFLOAT2 SrcPos = m_Position;
	DirectX::XMFLOAT2 SrcSize = m_Size;
	//const SlateBase* pRootParent = GetRootParent();
	//if (m_pParent != nullptr)
	//{
	//	SrcPos = m_pParent->GetPosition();
	//	SrcSize.x = m_pParent->GetWidth();
	//	SrcSize.y = m_pParent->GetHeight();
	//}
	auto&& pChild = m_pChildren[0];
	const FSlateInfos& childSlateInfos = pChild->GetSlateInfos();
	const float childWidth = pChild->GetWidth();
	const float childHeight = pChild->GetHeight();

	const auto& childPos = pChild->GetPosition();
	NewPos = childPos;

	if (childWidth == 0)
	{
		NewSize.x = cellW;
	}
	else
	{
		NewSize.x = childWidth;
	}
	switch (childSlateInfos.HAlign)
	{
	case EHorizontalAlignment::Left:
		NewPos.x = SrcPos.x + childSlateInfos.padding.left;
		break;
	case EHorizontalAlignment::Right:
		NewPos.x = SrcPos.x + SrcSize.x - NewSize.x - childSlateInfos.padding.right;
		break;
	case EHorizontalAlignment::Center:
		NewPos.x = SrcPos.x + (SrcSize.x / 2.f) - (NewSize.x / 2.f) + childSlateInfos.padding.left;
		break;
	default:
		NewPos.x = SrcPos.x + childSlateInfos.padding.left;
		break;
	}

	if (childHeight == 0)
	{
		NewSize.y = cellH;
	}
	else
	{
		NewSize.y = childHeight;
	}
	switch (childSlateInfos.VAlign)
	{
	case EVerticalAlignment::Top:
		NewPos.y = SrcPos.y + childSlateInfos.padding.top;
		break;
	case EVerticalAlignment::Bottom:
		NewPos.y = SrcPos.y + SrcSize.y - NewSize.y - childSlateInfos.padding.bottom;
		break;
	case EVerticalAlignment::Center:
		NewPos.y = SrcPos.y + (cellH / 2.f) - (NewSize.y / 2.f);// +childSlateInfos.padding.top;
		break;
	default:
		NewPos.y = SrcPos.y + childSlateInfos.padding.top;
		break;
	}
#if _DEBUG
	OutputDebugStringA(std::format("Overlay child size {}, {} pos {}, {}\n", NewSize.x, NewSize.y, NewPos.x, NewPos.y).c_str());
#endif
	pChild->SetSize(NewSize);
	pChild->SetPosition(NewPos);
	pChild->Draw();
#if _DEBUG
	m_pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Yellow)
	);
#endif
}