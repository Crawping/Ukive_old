#pragma once
#include "USpan.h"

class View;

class EffectSpan : public USpan
{
public:
	EffectSpan(unsigned int start, unsigned int end);
	virtual ~EffectSpan();

	D2D1_COLOR_F mTextColor;
	D2D1_COLOR_F mUnderlineColor;
	D2D1_COLOR_F mStrikethroughColor;

	int getBaseType();
	virtual int getType();

	virtual bool onDrawText(
		View *widget,
		float baselineOriginX, float baselineOriginY,
		DWRITE_GLYPH_RUN const* glyphRun, DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush);
	virtual bool onDrawUnderline(
		View *widget,
		float baselineOriginX, float baselineOriginY,
		DWRITE_UNDERLINE const* underline,
		ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush);
	virtual bool onDrawStrikethrough(
		View *widget,
		float baselineOriginX, float baselineOriginY,
		DWRITE_STRIKETHROUGH const* strikethrough,
		ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush);
	virtual bool onDrawInlineObject(
		View *widget,
		float originX, float originY,
		IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft,
		ID2D1RenderTarget *rt, ID2D1SolidColorBrush *brush);
};