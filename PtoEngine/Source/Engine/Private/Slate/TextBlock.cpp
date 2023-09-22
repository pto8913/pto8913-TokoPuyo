

#include "Slate/TextBlock.h"
#include "Slate/TextRenderer_Outline.h"

#include "Core/DirectX.h"

#include "Utils/String.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

#include "EngineSettings.h"

#include <wincodec.h>

S_TextBlock::S_TextBlock(ID2D1RenderTarget* inRt2D, DirectX11& dx, FVector2D inSize, FSlateInfos inSlateInfos, FSlateFont inFont, FSlateTextAppearance inAppearance)
	: SlateSlotBase(inRt2D, inSize, inSlateInfos), mText(L""), mFont(inFont), mAppearance(inAppearance)
{
	//D2D1CreateFactory(
	//	D2D1_FACTORY_TYPE_SINGLE_THREADED,
	//	&pD2DFactory
	//);
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
	);

	//pD2DFactory->CreateHwndRenderTarget(
	//	D2D1::RenderTargetProperties(),
	//	D2D1::HwndRenderTargetProperties(dx.GetHWnd()),
	//	&pRtHWnd
	//);

	pRt2D->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.color),
		&pBrush
	);
	mTextLength = (UINT32)mText.length();
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

	//if (mAppearance.outline)
	//{
	//	pRtHWnd->BeginDraw();
	//	pRtHWnd->Clear(D2D1_COLOR_F(1, 1, 1));

	//	pTextLayout->Draw(nullptr, pTextRenderer_Outline, mPosition.x, mPosition.y);
	//	pRtHWnd->EndDraw();
	//	return;
	//}

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

	//if (mAppearance.outline)
	//{
	//	pRt2D->CreateSolidColorBrush(
	//		ColorHelper::ConvertColorToD2D(mAppearance.outlineColor),
	//		&pBrushOutline
	//	);
	//	if (pTextRenderer_Outline != nullptr)
	//	{
	//		pTextRenderer_Outline->UpdateBrushFill(pBrush);
	//		pTextRenderer_Outline->UpdateBrushOutline(pBrushOutline);
	//	}
	//	else
	//	{
	//		pTextRenderer_Outline = new TextRenderer_Outline(
	//			pD2DFactory,
	//			pRtHWnd,
	//			pBrushOutline,
	//			pBrush
	//		);
	//	}
	//}
	//UpdateTextLayout();
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
	//UpdateTextLayout();
}
void S_TextBlock::SetText(std::wstring inText)
{
	mText = inText;
	mTextLength = (UINT32)mText.length();

	UpdateSize();
	//UpdateTextLayout();
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
//void S_TextBlock::UpdateTextLayout()
//{
//	pDWriteFactory->CreateTextLayout(
//		mText.c_str(),
//		mTextLength,
//		pTextFormat,
//		FLT_MAX,
//		0,
//		&pTextLayout
//	);
//}

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
