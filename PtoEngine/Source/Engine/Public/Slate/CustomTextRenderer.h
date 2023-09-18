#pragma once

#include <d2d1.h>
#include <dwrite.h>

class CustomTextRenderer
{
public:
    CustomTextRenderer(
        ID2D1Factory* pD2DFactory,
        ID2D1HwndRenderTarget* pRT,
        ID2D1SolidColorBrush* pOutlineBrush,
        ID2D1BitmapBrush* pFillBrush
    );

    void DrawGlyphRun(_In_ CONST DWRITE_GLYPH_RUN* glyphRun,
        float baselineOriginX,
        float baselineOriginY
    );
protected:
    int cRefCount_;
    ID2D1Factory* pD2DFactory_;
    ID2D1HwndRenderTarget* pRT_;
    ID2D1SolidColorBrush* pOutlineBrush_;
    ID2D1BitmapBrush* pFillBrush_;

};