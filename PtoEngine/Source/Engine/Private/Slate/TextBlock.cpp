

#include "Slate/TextBlock.h"
#include "Slate/CustomTextRenderer.h"

#include "Core/DirectX.h"

#include "Utils/String.h"

#include "Helper/ColorHelper.h"
#include "Helper/RectHelper.h"

#include "EngineSettings.h"

S_TextBlock::S_TextBlock(ID2D1RenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos, FSlateFont inFont, FSlateTextAppearance inAppearance)
	: SlateSlotBase(inRt2D, inSize, inSlateInfos), mText(L""), mFont(inFont), mAppearance(inAppearance)
{
	const auto windowSize = EngineSettings::GetWindowSize();

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	pRt2D->CreateSolidColorBrush(
		ColorHelper::ConvertColorToD2D(mAppearance.color),
		&pBrush
	);

	if (pBitmapBrush == nullptr)
	{
		D2D1_SIZE_U bitmapSize;
		bitmapSize.width = windowSize.x;
		bitmapSize.height = windowSize.y;
		D2D1_BITMAP_PROPERTIES bitmapProps;
		ID2D1Bitmap* pBitmap = nullptr;
		pRt2D->CreateBitmap(bitmapSize, bitmapProps, &pBitmap);
		// Create the bitmap brush
		D2D1_BITMAP_BRUSH_PROPERTIES properties = { D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR };
		pRt2D->CreateBitmapBrush(
			pBitmap,
			properties,
			&pBitmapBrush
		);
	}
	pCustomTextRenderer = new CustomTextRenderer(
		pD2DFactory,
		pRt2D,
		pBrush,
		pBitmapBrush
	);

	SetFont(mFont);
	SetAppearance(mAppearance);
}
S_TextBlock::S_TextBlock(ID2D1RenderTarget* inRt2D, FSlateInfos inSlateInfos, FSlateFont inFont, FSlateTextAppearance inAppearance)
	: S_TextBlock(inRt2D, { 0,0 }, inSlateInfos, inFont, inAppearance)
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
		pRt2D->DrawGeometry(pPathGeometry, pBrush, mAppearance.layoutOutlineWidth);
		pRt2D->FillGeometry(pPathGeometry, pBrush);
		return;
	}
	pTextLayout->Draw(NULL, pCustomTextRenderer, mPosition.x, mPosition.y);

	//pRt2D->DrawText(
	//	mText.c_str(),
	//	(UINT32)mText.size(),
	//	pTextFormat,
	//	RectHelper::ConvertRectToD2D(GetRect()),
	//	pBrush
	//);
}

void S_TextBlock::SetAppearance(FSlateTextAppearance in)
{
	mAppearance = in;

	SetAppearHorizontalAlignment(mAppearance.hAlign);
	SetAppearVerticalAlignment(mAppearance.vAlign);
	SetWrap(mAppearance.wrap);

	UpdateTextLayout();
	if (mAppearance.layoutOutline)
	{
		SetOutline();
	}

	pBrush->SetColor(ColorHelper::ConvertColorToD2D(mAppearance.color));
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
	SetOutline();
}
void S_TextBlock::SetText(std::wstring inText)
{
	mText = inText;

	UpdateSize();
	UpdateTextLayout();
	SetOutline();
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
void S_TextBlock::UpdateTextLayout()
{
	const auto windowSize = EngineSettings::GetWindowSize();
	pDWriteFactory->CreateTextLayout(
		mText.c_str(),
		mText.length(),
		pTextFormat,
		windowSize.x,
		windowSize.y,
		&pTextLayout
	);
	DWRITE_TEXT_RANGE textRange;
	textRange.length = mText.length();
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
void S_TextBlock::SetOutline()
{
	IDWriteFontFile* pFontFile = nullptr;
	pDWriteFactory->CreateFontFileReference(
		mFont.fileName.c_str(),
		NULL,
		&pFontFile
	);
	IDWriteFontFile* pFontFiles[] = { pFontFile };
	
	IDWriteFontFace* pFontFace = nullptr;
	pDWriteFactory->CreateFontFace(
		DWRITE_FONT_FACE_TYPE_TRUETYPE,
		1,
		pFontFiles,
		0,
		DWRITE_FONT_SIMULATIONS_NONE,
		&pFontFace
	);

	const auto textLength = mText.length();
	UINT* pCodePoints = new UINT[textLength];
	UINT16* pGlyphIndices = new UINT16[textLength];
	ZeroMemory(pCodePoints, sizeof(UINT) * textLength);
	ZeroMemory(pGlyphIndices, sizeof(UINT16) * textLength);
	for (int i = 0; i < textLength; ++i)
	{
		pCodePoints[i] = mText.at(i);
	}
	pFontFace->GetGlyphIndicesW(pCodePoints, textLength, pGlyphIndices);

	pD2DFactory->CreatePathGeometry(&pPathGeometry);
	pPathGeometry->Open(&pGeometrySink);
	pFontFace->GetGlyphRunOutline(
		96.f,
		pGlyphIndices,
		NULL,
		NULL,
		textLength,
		FALSE,
		FALSE,
		pGeometrySink
	);
	pGeometrySink->Close();
	if (pCodePoints)
	{
		delete[] pCodePoints;
		pCodePoints = nullptr;
	}
	if (pGlyphIndices)
	{
		delete[] pGlyphIndices;
		pGlyphIndices = nullptr;
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