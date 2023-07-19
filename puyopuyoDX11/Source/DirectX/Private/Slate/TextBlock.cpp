
#include "Slate/TextBlock.h"

#include "Core/DirectX.h"

#include "Utils/String.h"

S_TextBlock::S_TextBlock(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos, FSlateFont inFont, FSlateTextAppearance inAppearance)
	: SlateSlotBase(dx, { m_Font.fontSize * m_Text.size(), m_Font.fontSize}, inD2DRT, inSlateInfos), m_Text(L"TextBlock"), m_Font(inFont), m_Appearance(inAppearance)
{
	m_pDWriteFactory = dx.GetDWriteFactory();

	m_pD2DRenderTarget->CreateSolidColorBrush(m_Appearance.color, &m_pBrush);

	SetFont(m_Font);
	SetAppearance(m_Appearance);
}

void S_TextBlock::Draw()
{
	m_pD2DRenderTarget->DrawText(
		m_Text.c_str(),
		(UINT32)m_Text.size(),
		m_pTextFormat,
		GetRect(),
		m_pBrush
	);
#if _DEBUG
	m_pD2DRenderTarget->DrawRectangle(GetRect(), m_pBrush);
#endif
}

void S_TextBlock::SetFont(FSlateFont inFont)
{
	m_Font = inFont;

	m_pDWriteFactory->CreateTextFormat(
		m_Font.fontFamily,
		NULL,
		m_Font.weight,
		m_Font.style,
		m_Font.stretch,
		m_Font.fontSize,
		m_Font.fontLocalName,
		&m_pTextFormat
	);
	SetSize(
		{
			m_Font.fontSize * m_Text.size(),
			0
		}
	);
}
void S_TextBlock::SetAppearance(FSlateTextAppearance in)
{
	m_Appearance = in;

	SetAppearHorizontalAlignment(m_Appearance.hAlign);
	SetAppearVerticalAlignment(m_Appearance.vAlign);
	SetWrap(m_Appearance.wrap);

	m_pBrush->SetColor(m_Appearance.color);
}
void S_TextBlock::SetText(std::wstring inText)
{
	m_Text = inText;
	auto lines = Util::Split(m_Text, L'\n');
#if _DEBUG
	OutputDebugStringA(std::format("{}", lines.size()).c_str());
#endif
	UINT8 lineCount = (UINT8)lines.size();
	if (lineCount > 1)
	{
		SetSize(
			{
				m_Font.fontSize * lines[0].size(),
				m_Font.fontSize * lines.size()
			}
		);

	}
	//SetSize(
	//	{
	//		m_Font.fontSize * m_Text.size(),
	//		0
	//	}
	//);
}
void S_TextBlock::SetSize(DirectX::XMFLOAT2 inSize)
{
	m_Size.x = inSize.x;
	m_Size.y = m_Font.fontSize;
}

void S_TextBlock::SetAppearHorizontalAlignment(EHorizontalAlignment in)
{
	m_Appearance.hAlign = in;
	switch (in)
	{
	case EHorizontalAlignment::Left:
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case EHorizontalAlignment::Right:
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	default:
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	}
}
void S_TextBlock::SetAppearVerticalAlignment(EVerticalAlignment in)
{
	m_Appearance.vAlign = in;
	switch (in)
	{
	case EVerticalAlignment::Top:
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case EVerticalAlignment::Bottom:
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	default:
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	}
}

void S_TextBlock::SetWrap(ETextWrap in)
{
	m_Appearance.wrap = in;
	switch (in)
	{
	case ETextWrap::No:
		m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		break;
	case ETextWrap::Word:
		m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WHOLE_WORD);
		break;
	case ETextWrap::Character:
		m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
		break;
	case ETextWrap::BreakWord:
		m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK);
		break;
	default:
		m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		break;
	}
}