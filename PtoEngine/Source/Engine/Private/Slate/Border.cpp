
#include "Slate/Border.h"
#include <wincodec.h>

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

S_Border::S_Border(ID2D1HwndRenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos, FSlateBorderAppearance inAppearance)
	: SlotContainerOnlyOne(inRt2D, inSize, inSlateInfos)
{
	SetAppearance(inAppearance);
#if _DEBUG
	pBrush->SetColor(
		D2D1::ColorF(D2D1::ColorF::Yellow)
	);
#endif
}
S_Border::S_Border(ID2D1HwndRenderTarget* inRt2D, FSlateInfos inSlateInfos, FSlateBorderAppearance inAppearance)
	: S_Border(inRt2D, { 0,0 }, inSlateInfos, inAppearance)
{
}
S_Border::~S_Border()
{
	Util::SafeRelease(pBitmap);
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void S_Border::Draw()
{
	if (!bIsVisible)
	{
		return;
	}

	pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.color));
	switch (mAppearance.Type)
	{
	case EBorderType::None:
		SlotContainerOnlyOne::Draw();
		return;
	case EBorderType::Box:
		if (mAppearance.bIsFill)
		{
			pRt2D->FillRectangle(
				RectHelper::ConvertRectToD2D(GetRect()),
				pBrush
			);
		}
		else
		{
			pRt2D->DrawRectangle(
				RectHelper::ConvertRectToD2D(GetRect()),
				pBrush,
				mAppearance.lineWidth
			);
		}
		break;
	case EBorderType::Border:
		if (mAppearance.bIsFill)
		{
			pRt2D->FillRoundedRectangle(
				D2D1::RoundedRect(
					RectHelper::ConvertRectToD2D(GetRect()),
					mAppearance.roundSize.x,
					mAppearance.roundSize.y
				),
				pBrush
			);
		}
		else
		{
			pRt2D->DrawRoundedRectangle(
				D2D1::RoundedRect(
					RectHelper::ConvertRectToD2D(GetRect()),
					mAppearance.roundSize.x,
					mAppearance.roundSize.y
				),
				pBrush,
				mAppearance.lineWidth
			);
		}
		break;
	case EBorderType::Image:
		if (pBrush != nullptr)
		{
			pRt2D->DrawBitmap(
				pBitmap,
				RectHelper::ConvertRectToD2D(GetRect()),
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
	mAppearance = inAppearance;

	pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.color));
}
FSlateBorderAppearance& S_Border::GetAppearance()
{
	return mAppearance;
}

void S_Border::SetFileName(std::wstring in)
{
	if (in.empty())
	{
		return;
	}
	if (mAppearance.fileName == in)
	{
		return;
	}
	mAppearance.fileName = in;

	if (pBrush)
	{
		pBrush->Release();
		pBrush = nullptr;
	}

	IWICImagingFactory* pImageFactory;
	IWICBitmapDecoder* pBrushDecoder;
	IWICBitmapFrameDecode* pBrushFrameDecode;
	IWICFormatConverter* pImageConverter;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pImageFactory);
	pImageFactory->CreateDecoderFromFilename(in.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBrushDecoder);
	pBrushDecoder->GetFrame(0, &pBrushFrameDecode);
	pImageFactory->CreateFormatConverter(&pImageConverter);
	pImageConverter->Initialize(pBrushFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 1.f, WICBitmapPaletteTypeMedianCut);

	pRt2D->CreateBitmapFromWicBitmap(pImageConverter, nullptr, &pBitmap);

	pImageFactory->Release();
	pBrushDecoder->Release();
	pBrushFrameDecode->Release();
	pImageConverter->Release();
}

void S_Border::Update()
{
	if (GetChildrenCount() == 0) return;

	const float cellH = GetHeight();
	const float cellW = GetWidth();

	FVector2D NewSize = { 0, 0 };
	FVector2D NewPos = { 0, 0 };

	FVector2D SrcPos = mPosition;
	FVector2D SrcSize = mSize;
	//const SlateBase* pRootParent = GetRootParent();
	//if (pParent != nullptr)
	//{
	//	SrcPos = pParent->GetPosition();
	//	SrcSize.x = pParent->GetWidth();
	//	SrcSize.y = pParent->GetHeight();
	//}
	auto&& pChild = pChildren[0];
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
	pChild->SetSize(NewSize);
	pChild->SetPosition(NewPos);
	pChild->Draw();

}