#include "UCommon.h"
#include "UWidget.h"
#include "UColor.h"
#include "UMath.h"
#include "UDeviceManager.h"
#include "EffectSpan.h"
#include "UTextView.h"
#include "UnderlineSpan.h"
#include "UTextDrawingEffect.h"
#include "UTextRenderer.h"


UTextRenderer::UTextRenderer(UComPtr<ID2D1RenderTarget> renderTarget)
	:mRefCount(1),
	mOpacity(1.f)
{
	mDefaultTextColor = UColor::Black;
	mDefaultUnderlineColor = UColor::Black;
	mDefaultStrikethroughColor = UColor::Black;

	mRenderTarget = renderTarget;
	mSolidBrush = UDeviceManager::sCanvasSolidBrush;
}


UTextRenderer::~UTextRenderer()
{
}


void UTextRenderer::setOpacity(float opacity)
{
	mOpacity = opacity;
	mSolidBrush->SetOpacity(opacity);
}

void UTextRenderer::setTextColor(D2D1_COLOR_F color)
{
	mDefaultTextColor = color;
}

void UTextRenderer::setUnderlineColor(D2D1_COLOR_F color)
{
	mDefaultUnderlineColor = color;
}

void UTextRenderer::setStrikethroughColor(D2D1_COLOR_F color)
{
	mDefaultStrikethroughColor = color;
}


/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawGlyphRun                               *
*                                                                 *
*  Gets GlyphRun outlines via IDWriteFontFace::GetGlyphRunOutline *
*  and then draws and fills them using Direct2D path geometries   *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::DrawGlyphRun(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	DWRITE_MEASURING_MODE measuringMode,
	__in DWRITE_GLYPH_RUN const* glyphRun,
	__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
	IUnknown* clientDrawingEffect
)
{
	if (clientDrawingContext != nullptr)
	{
		URectF region;
		UTextView *textView = (UTextView*)clientDrawingContext;
		textView->computeVisibleRegion(&region);

		float dpiX, dpiY;
		mRenderTarget->GetDpi(&dpiX, &dpiY);

		if ((int)UMath::floor(baselineOriginX) > (int)UMath::ceil(region.right))
			return S_OK;

		if ((int)UMath::ceil(baselineOriginY) < (int)UMath::floor(region.top))
			return S_OK;
		if ((int)UMath::floor(baselineOriginY - glyphRun->fontEmSize*(dpiY / 96.f)) > (int)UMath::ceil(region.bottom))
			return S_OK;
	}

	if (clientDrawingEffect != nullptr)
	{
		UComPtr<UTextDrawingEffect> drawingEffect;
		clientDrawingEffect->QueryInterface(__uuidof(UTextDrawingEffect), reinterpret_cast<void**>(&drawingEffect));

		EffectSpan *span = drawingEffect->mEffectSpan;
		mSolidBrush->SetColor(span->mTextColor);

		if (!span->onDrawText(
			(UWidget*)clientDrawingContext,
			baselineOriginX, baselineOriginY,
			glyphRun, glyphRunDescription,
			mRenderTarget.get(), mSolidBrush.get()))
		{
			mRenderTarget->DrawGlyphRun(
				D2D1::Point2F(baselineOriginX, baselineOriginY),
				glyphRun, mSolidBrush.get(), measuringMode);
		}
	}
	else
	{
		mSolidBrush->SetColor(mDefaultTextColor);

		mRenderTarget->DrawGlyphRun(
			D2D1::Point2F(baselineOriginX, baselineOriginY),
			glyphRun, mSolidBrush.get(), measuringMode);
	}

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawUnderline                              *
*                                                                 *
*  Draws underlines below the text using a Direct2D recatangle    *
*  geometry                                                       *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::DrawUnderline(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	__in DWRITE_UNDERLINE const* underline,
	IUnknown* clientDrawingEffect
)
{
	D2D1_RECT_F rect = D2D1::RectF(
		0 + baselineOriginX,
		underline->offset + baselineOriginY,
		underline->width + baselineOriginX,
		underline->offset + underline->thickness + baselineOriginY);

	if (clientDrawingEffect != 0)
	{
		UComPtr<UTextDrawingEffect> drawingEffect;
		clientDrawingEffect->QueryInterface(__uuidof(UTextDrawingEffect), reinterpret_cast<void**>(&drawingEffect));

		EffectSpan *span = drawingEffect->mEffectSpan;
		mSolidBrush->SetColor(span->mUnderlineColor);

		if (!span->onDrawUnderline(
			(UWidget*)clientDrawingContext,
			baselineOriginX, baselineOriginY,
			underline,
			mRenderTarget.get(), mSolidBrush.get()))
			mRenderTarget->FillRectangle(rect, mSolidBrush.get());
	}
	else
	{
		mSolidBrush->SetColor(mDefaultUnderlineColor);
		mRenderTarget->FillRectangle(rect, mSolidBrush.get());
	}

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawStrikethrough                          *
*                                                                 *
*  Draws strikethroughs below the text using a Direct2D           *
*  recatangle geometry                                            *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::DrawStrikethrough(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	__in DWRITE_STRIKETHROUGH const* strikethrough,
	IUnknown* clientDrawingEffect
)
{
	D2D1_RECT_F rect = D2D1::RectF(
		0 + baselineOriginX,
		strikethrough->offset + baselineOriginY,
		strikethrough->width + baselineOriginX,
		strikethrough->offset + strikethrough->thickness + baselineOriginY);

	if (clientDrawingEffect != 0)
	{
		UComPtr<UTextDrawingEffect> drawingEffect;
		clientDrawingEffect->QueryInterface(__uuidof(UTextDrawingEffect), reinterpret_cast<void**>(&drawingEffect));

		EffectSpan *span = drawingEffect->mEffectSpan;
		mSolidBrush->SetColor(span->mStrikethroughColor);

		if (!span->onDrawStrikethrough(
			(UWidget*)clientDrawingContext,
			baselineOriginX, baselineOriginY,
			strikethrough,
			mRenderTarget.get(), mSolidBrush.get()))
			mRenderTarget->FillRectangle(rect, mSolidBrush.get());
	}
	else
	{
		mSolidBrush->SetColor(mDefaultStrikethroughColor);
		mRenderTarget->FillRectangle(rect, mSolidBrush.get());
	}

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawInlineObject                           *
*                                                                 *
*  This function is not implemented for the purposes of this      *
*  sample.                                                        *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::DrawInlineObject(
	__maybenull void* clientDrawingContext,
	FLOAT originX,
	FLOAT originY,
	IDWriteInlineObject* inlineObject,
	BOOL isSideways,
	BOOL isRightToLeft,
	IUnknown* clientDrawingEffect
)
{
	// Not implemented
	return E_NOTIMPL;
}


STDMETHODIMP_(unsigned long) UTextRenderer::AddRef()
{
	return InterlockedIncrement(&mRefCount);
}


STDMETHODIMP_(unsigned long) UTextRenderer::Release()
{
	unsigned long newCount = InterlockedDecrement(&mRefCount);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::IsPixelSnappingDisabled                    *
*                                                                 *
*  Determines whether pixel snapping is disabled. The recommended *
*  default is FALSE, unless doing animation that requires         *
*  subpixel vertical placement.                                   *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::IsPixelSnappingDisabled(
	__maybenull void* clientDrawingContext,
	__out BOOL* isDisabled
)
{
	*isDisabled = FALSE;
	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::GetCurrentTransform                        *
*                                                                 *
*  Returns the current transform applied to the render target..   *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::GetCurrentTransform(
	__maybenull void* clientDrawingContext,
	__out DWRITE_MATRIX* transform
)
{
	//forward the render target's transform
	mRenderTarget->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::GetPixelsPerDip                            *
*                                                                 *
*  This returns the number of pixels per DIP.                     *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::GetPixelsPerDip(
	__maybenull void* clientDrawingContext,
	__out FLOAT* pixelsPerDip
)
{
	float x, yUnused;

	mRenderTarget->GetDpi(&x, &yUnused);
	*pixelsPerDip = x / 96;

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::QueryInterface                             *
*                                                                 *
*  Query interface implementation                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP UTextRenderer::QueryInterface(
	IID const& riid,
	void** ppvObject
)
{
	if (__uuidof(IDWriteTextRenderer) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IDWritePixelSnapping) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IUnknown) == riid)
	{
		*ppvObject = this;
	}
	else
	{
		*ppvObject = 0;
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}