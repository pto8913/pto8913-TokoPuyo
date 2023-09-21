
#include "Slate/TextRenderer_Outline.h"
#include "Utils/DirectXUtils.h"

TextRenderer_Outline::TextRenderer_Outline(ID2D1Factory* inD2DFactory, ID2D1HwndRenderTarget* inRT, ID2D1SolidColorBrush* inBrushOutline, ID2D1SolidColorBrush* inBrushFill)
    :
    cRefCount_(0),
    pD2DFactory(inD2DFactory),
    pRtHWnd(inRT),
    pBrushOutline(inBrushOutline),
    pBrushFill(inBrushFill)
{
    pD2DFactory->AddRef();
    pRtHWnd->AddRef();
    pBrushOutline->AddRef();
    pBrushFill->AddRef();
}
TextRenderer_Outline::~TextRenderer_Outline()
{
    Util::SafeRelease(pD2DFactory);
    Util::SafeRelease(pRtHWnd);
    Util::SafeRelease(pBrushOutline);
    Util::SafeRelease(pBrushFill);
}

void TextRenderer_Outline::UpdateBrushFill(ID2D1SolidColorBrush* inBrushFill)
{
    pBrushFill= inBrushFill;
}
void TextRenderer_Outline::UpdateBrushOutline(ID2D1SolidColorBrush* inBrushOutline)
{
    pBrushOutline = inBrushOutline;
}

IFACEMETHODIMP TextRenderer_Outline::DrawGlyphRun(
    __maybenull void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    DWRITE_MEASURING_MODE measuringMode,
    __in DWRITE_GLYPH_RUN const* glyphRun,
    __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
    IUnknown* clientDrawingEffect
)
{
    ID2D1GeometrySink* pSink = nullptr;
    ID2D1PathGeometry* pTextGeometry = nullptr;
    pD2DFactory->CreatePathGeometry(&pTextGeometry);
    pTextGeometry->Open(&pSink);

    // Get the glyph run outline geometries back from DirectWrite and place them within the
    // geometry sink.
    glyphRun->fontFace->GetGlyphRunOutline(
        glyphRun->fontEmSize,
        glyphRun->glyphIndices,
        glyphRun->glyphAdvances,
        glyphRun->glyphOffsets,
        glyphRun->glyphCount,
        glyphRun->isSideways,
        glyphRun->bidiLevel % 2,
        pSink
    );

    // Close the geometry sink
    pSink->Close();

    // Initialize a matrix to translate the origin of the glyph run.
    D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
        1.0f, 0.0f,
        0.0f, 1.0f,
        baselineOriginX, baselineOriginY
    );

    // Create the transformed geometry
    ID2D1TransformedGeometry* pTransformedGeometry = NULL;
    pD2DFactory->CreateTransformedGeometry(
        pTextGeometry,
        &matrix,
        &pTransformedGeometry
    );
    // Draw the outline of the glyph run
    pRtHWnd->DrawGeometry(
        pTransformedGeometry,
        pBrushOutline
    );

    // Fill in the glyph run
    pRtHWnd->FillGeometry(
        pTransformedGeometry,
        pBrushFill
    );

    //pRtHWnd->DrawGlyphRun(D2D1_POINT_2F(baselineOriginX, baselineOriginY), glyphRun, pBrushOutline.get(), measuringMode);

    pRtHWnd->SetTransform(D2D1::IdentityMatrix()); // make sure we come back to no transform

    return S_OK;
}
