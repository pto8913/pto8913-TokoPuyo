#pragma once

#include "SlateSlotBase.h"
#include <dwrite.h>

class DirectX11;

DECLARE_MULTICAST_DELEGATE_RET(FOnSetText, std::wstring);

struct FSlateFont
{
public:
	FSlateFont() 
		: fontFamily(L"Script"), 
		fontLocalName(L"en-us"),
		fontSize(24.f), 
		weight(DWRITE_FONT_WEIGHT_REGULAR), 
		style(DWRITE_FONT_STYLE_NORMAL),
		stretch(DWRITE_FONT_STRETCH_NORMAL)
	{}
	
	const wchar_t* fontFamily;
	const wchar_t* fontLocalName;
	float fontSize;

	DWRITE_FONT_WEIGHT weight;
	DWRITE_FONT_STYLE style;
	DWRITE_FONT_STRETCH stretch;
};

enum class ETextWrap
{
	No,
	Word,
	Character,
	Rect,
	BreakWord
};
struct FSlateTextAppearance
{
public:
	FSlateTextAppearance()
		: color(D2D1::ColorF(255.f, 255.f, 255.f, 1.f)),
		hAlign(EHorizontalAlignment::Center),
		vAlign(EVerticalAlignment::Center),
		wrap(ETextWrap::No)
	{}

	D2D1_COLOR_F color;

	EHorizontalAlignment hAlign;
	EVerticalAlignment vAlign;

	ETextWrap wrap;
};

/*  
* If bitmap nullptr, the parent slate must be execute these row.
*
*		m_pContext->SetTarget(m_pBitmap);
*		m_pContext->BeginDraw();
*
*				~~~
*			 something
*
*				~~~
*		m_pContext->EndDraw();
*
*/
class S_TextBlock : public SlateSlotBase
{
public:
	S_TextBlock(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateFont inFont = {}, FSlateTextAppearance inAppearance = {});
	virtual ~S_TextBlock();

	virtual void Draw() override;

	void SetFont(FSlateFont inFont);
	void SetAppearance(FSlateTextAppearance in);
	void SetText(std::wstring inText);

	virtual void SetSize(DirectX::XMFLOAT2 inSize) override;

	virtual void SetAppearHorizontalAlignment(EHorizontalAlignment in);
	virtual void SetAppearVerticalAlignment(EVerticalAlignment in);

	virtual void SetWrap(ETextWrap in);

	/* NOTE : this will be called per frame. */
	FOnSetText OnSetText;

protected:
	virtual void UpdateSize();
private:

	FSlateTextAppearance m_Appearance;

	IDWriteTextFormat* m_pTextFormat = nullptr;
	FSlateFont m_Font;

	std::wstring m_Text;
};