
#include "Slate/CustomTextRenderer.h"

CustomTextRenderer::CustomTextRenderer(
    ID2D1Factory* pD2DFactory,
    ID2D1HwndRenderTarget* pRT,
    ID2D1SolidColorBrush* pOutlineBrush,
    ID2D1BitmapBrush* pFillBrush
)
    :
    cRefCount_(0),
    pD2DFactory_(pD2DFactory),
    pRT_(pRT),
    pOutlineBrush_(pOutlineBrush),
    pFillBrush_(pFillBrush)
{
    pD2DFactory_->AddRef();
    pRT_->AddRef();
    pOutlineBrush_->AddRef();
    pFillBrush_->AddRef();
}

void CustomTextRenderer::DrawGlyphRun(
    _In_ CONST DWRITE_GLYPH_RUN* glyphRun,
    float baselineOriginX,
    float baselineOriginY
)
{
    // Create the path geometry.
    ID2D1PathGeometry* pPathGeometry = NULL;
    HRESULT hr = pD2DFactory_->CreatePathGeometry(
        &pPathGeometry
    );

    // Write to the path geometry using the geometry sink.
    ID2D1GeometrySink* pSink = NULL;
    if (SUCCEEDED(hr))
    {
        hr = pPathGeometry->Open(
            &pSink
        );
    }

    // Get the glyph run outline geometries back from DirectWrite and place them within the
    // geometry sink.
    if (SUCCEEDED(hr))
    {
        hr = glyphRun->fontFace->GetGlyphRunOutline(
            glyphRun->fontEmSize,
            glyphRun->glyphIndices,
            glyphRun->glyphAdvances,
            glyphRun->glyphOffsets,
            glyphRun->glyphCount,
            glyphRun->isSideways,
            glyphRun->bidiLevel % 2,
            pSink
        );
    }
    // Close the geometry sink
    if (SUCCEEDED(hr))
    {
        hr = pSink->Close();
    }

    // Initialize a matrix to translate the origin of the glyph run.
    D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
        1.0f, 0.0f,
        0.0f, 1.0f,
        baselineOriginX, baselineOriginY
    );

    // Create the transformed geometry
    ID2D1TransformedGeometry* pTransformedGeometry = NULL;
    if (SUCCEEDED(hr))
    {
        hr = pD2DFactory_->CreateTransformedGeometry(
            pPathGeometry,
            &matrix,
            &pTransformedGeometry
        );
    }
    // Draw the outline of the glyph run
    pRT_->DrawGeometry(
        pTransformedGeometry,
        pOutlineBrush_
    );

    // Fill in the glyph run
    pRT_->FillGeometry(
        pTransformedGeometry,
        pFillBrush_
    );
}
