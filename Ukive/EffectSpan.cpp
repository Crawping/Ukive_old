#include "UCommon.h"
#include "View.h"
#include "EffectSpan.h"


EffectSpan::EffectSpan(unsigned int start, unsigned int end)
	:USpan(start, end)
{
	mTextColor = D2D1::ColorF(D2D1::ColorF::Black);
	mUnderlineColor = D2D1::ColorF(D2D1::ColorF::Black);
	mStrikethroughColor = D2D1::ColorF(D2D1::ColorF::Black);
}

EffectSpan::~EffectSpan()
{
}


int EffectSpan::getType()
{
	return USpan::EFFECT_NORMAL;
}

int EffectSpan::getBaseType()
{
	return USpan::USPAN_BASE_TYPE_EFFECT;
}


bool EffectSpan::onDrawText(
	View *widget,
	float baselineOriginX, float baselineOriginY,
	DWRITE_GLYPH_RUN const* glyphRun, DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
	ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush)
{
	return false;
}

bool EffectSpan::onDrawUnderline(
	View *widget,
	float baselineOriginX, float baselineOriginY,
	DWRITE_UNDERLINE const* underline,
	ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush)
{
	return false;
}

bool EffectSpan::onDrawStrikethrough(
	View *widget,
	float baselineOriginX, float baselineOriginY,
	DWRITE_STRIKETHROUGH const* strikethrough,
	ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush)
{
	return false;
}

bool EffectSpan::onDrawInlineObject(
	View *widget,
	float originX, float originY,
	IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft,
	ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush)
{
	return false;
}