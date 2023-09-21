#pragma once

#include <d2d1.h>
#include <dwrite.h>

class TextRenderer_Outline : public IDWriteTextRenderer
{
public:
    TextRenderer_Outline(
        ID2D1Factory* inD2DFactory,
        ID2D1HwndRenderTarget* inRT,
        ID2D1SolidColorBrush* inBrushOutline,
        ID2D1SolidColorBrush* inBrushFill
    );
    virtual ~TextRenderer_Outline();

    void UpdateBrushFill(ID2D1SolidColorBrush* inBrush);
    void UpdateBrushOutline(ID2D1SolidColorBrush* inBrushOutline);

    IFACEMETHOD(DrawGlyphRun)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        DWRITE_MEASURING_MODE measuringMode,
        __in DWRITE_GLYPH_RUN const* glyphRun,
        __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
        IUnknown* clientDrawingEffect
    );

    STDMETHOD(IsPixelSnappingDisabled)(void* clientDrawingContext, BOOL* isDisabled)
    {
        *isDisabled = TRUE;
        return S_OK;
    }

    // nothing is implemented except IUnknown
    STDMETHOD(GetCurrentTransform)(void* clientDrawingContext, DWRITE_MATRIX* transform) { return E_NOTIMPL; }
    STDMETHOD(GetPixelsPerDip)(void* clientDrawingContext, FLOAT* pixelsPerDip) { return E_NOTIMPL; }
    STDMETHOD(DrawUnderline)(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_UNDERLINE const* underline, IUnknown* clientDrawingEffect) { return E_NOTIMPL; }
    STDMETHOD(DrawStrikethrough)(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_STRIKETHROUGH const* strikethrough, IUnknown* clientDrawingEffect) { return E_NOTIMPL; }
    STDMETHOD(DrawInlineObject)(void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect) { return E_NOTIMPL; }
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject)
    {
        if (riid == __uuidof(IUnknown)) { *ppvObject = (IUnknown*)this; return S_OK; }
        if (riid == __uuidof(IDWritePixelSnapping)) { *ppvObject = (IDWritePixelSnapping*)this; return S_OK; }
        if (riid == __uuidof(IDWriteTextRenderer)) { *ppvObject = (IDWriteTextRenderer*)this; return S_OK; }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG AddRef() { return 1; }
    ULONG Release() { return 1; }
private:
    unsigned long cRefCount_;
    ID2D1Factory* pD2DFactory = nullptr;
    ID2D1HwndRenderTarget* pRtHWnd = nullptr;
    ID2D1SolidColorBrush* pBrushOutline = nullptr;
    ID2D1SolidColorBrush* pBrushFill = nullptr;
};