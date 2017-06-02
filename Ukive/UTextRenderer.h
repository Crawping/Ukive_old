#pragma once
#include "UComPtr.h"

class UTextRenderer : public IDWriteTextRenderer
{
private:
	ULONG mRefCount;
	float mOpacity;

	D2D1_COLOR_F mDefaultTextColor;
	D2D1_COLOR_F mDefaultUnderlineColor;
	D2D1_COLOR_F mDefaultStrikethroughColor;

	UComPtr<ID2D1RenderTarget> mRenderTarget;
	UComPtr<ID2D1SolidColorBrush> mSolidBrush;

public:
	UTextRenderer(UComPtr<ID2D1RenderTarget> renderTarget);
	~UTextRenderer();

	void setOpacity(float opacity);
	void setTextColor(D2D1_COLOR_F color);
	void setUnderlineColor(D2D1_COLOR_F color);
	void setStrikethroughColor(D2D1_COLOR_F color);

	STDMETHOD(IsPixelSnappingDisabled)(
		__maybenull void* clientDrawingContext,
		__out BOOL* isDisabled
		);

	STDMETHOD(GetCurrentTransform)(
		__maybenull void* clientDrawingContext,
		__out DWRITE_MATRIX* transform
		);

	STDMETHOD(GetPixelsPerDip)(
		__maybenull void* clientDrawingContext,
		__out FLOAT* pixelsPerDip
		);

	STDMETHOD(DrawGlyphRun)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		__in DWRITE_GLYPH_RUN const* glyphRun,
		__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawUnderline)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawStrikethrough)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawInlineObject)(
		__maybenull void* clientDrawingContext,
		FLOAT originX,
		FLOAT originY,
		IDWriteInlineObject* inlineObject,
		BOOL isSideways,
		BOOL isRightToLeft,
		IUnknown* clientDrawingEffect
		);

public:
	unsigned long STDMETHODCALLTYPE AddRef();
	unsigned long STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(
		IID const& riid,
		void** ppvObject
	);
};