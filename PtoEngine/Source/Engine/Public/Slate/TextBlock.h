#pragma once

#include "SlateSlotBase.h"
#include <dwrite.h>

#include "Engine/Color.h"

class DirectX11;
class TextRenderer_Outline;

DECLARE_MULTICAST_DELEGATE_RET(FOnSetText, std::wstring);

struct FSlateFont
{
public:
	FSlateFont() 
		: fileName(L"Content/Fonts/khurasan-NewRamen.ttf"),
		fontFamily(L"Script"), 
		fontLocalName(L"en-us"),
		fontSize(24.f), 
		weight(DWRITE_FONT_WEIGHT_REGULAR), 
		style(DWRITE_FONT_STYLE_NORMAL),
		stretch(DWRITE_FONT_STRETCH_NORMAL)
	{}
	
	std::wstring fileName;
	std::wstring fontFamily;
	std::wstring fontLocalName;
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
		: color(FColor(1.f, 1.f, 1.f, 1.f)),
		hAlign(EHorizontalAlignment::Center),
		vAlign(EVerticalAlignment::Center),
		wrap(ETextWrap::No),
		outline(false),
		outlineColor(FColor(0.f, 0.f, 0.f, 1.f))
	{}

	FColor color;

	EHorizontalAlignment hAlign;
	EVerticalAlignment vAlign;

	ETextWrap wrap;

	// ------------------------------------------
	// Layout : Outline
	// ------------------------------------------
	bool outline;
	FColor outlineColor;
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
	S_TextBlock(ID2D1RenderTarget* inRt2D, DirectX11& dx, FVector2D inSize, FSlateInfos inSlateInfos = {}, FSlateFont inFont = {}, FSlateTextAppearance inAppearance = {});
	S_TextBlock(ID2D1RenderTarget* inRt2D, DirectX11& dx, FSlateInfos inSlateInfos = {}, FSlateFont inFont = {}, FSlateTextAppearance inAppearance = {});
	virtual ~S_TextBlock();

	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_TextBlock";
		}
		return "S_TextBlock";
	}
	virtual void Draw() override;

	void SetAppearance(FSlateTextAppearance in);
	FSlateTextAppearance& GetAppearance();

	void SetFont(FSlateFont inFont);
	void SetText(std::wstring inText);
	virtual void SetSize(FVector2D inSize) override;
protected:
	virtual void UpdateSize();
	/* Call when any of mText or pFormat is changed. */
	void UpdateTextLayout();
public:
	virtual void SetAppearHorizontalAlignment(EHorizontalAlignment in);
	virtual void SetAppearVerticalAlignment(EVerticalAlignment in);

	virtual void SetWrap(ETextWrap in);

	/* NOTE : this will be called per frame. */
	FOnSetText OnSetText;

protected:
	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	FSlateTextAppearance mAppearance;

	IDWriteFactory* pDWriteFactory = nullptr;
	IDWriteTextFormat* pTextFormat = nullptr;

	FSlateFont mFont;

	std::wstring mText;
	UINT32 mTextLength;

	// --------------------------------------------------
	// State : CustomTextRenderer
	// --------------------------------------------------
	ID2D1Factory* pD2DFactory = nullptr;
	ID2D1HwndRenderTarget* pRtHWnd = nullptr;
	ID2D1SolidColorBrush* pBrushOutline = nullptr;
	IDWriteTextLayout* pTextLayout = nullptr;

	TextRenderer_Outline* pTextRenderer_Outline = nullptr;

};