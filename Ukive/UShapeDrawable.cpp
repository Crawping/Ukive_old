#include "UCommon.h"
#include "UCanvas.h"
#include "UShapeDrawable.h"


UShapeDrawable::UShapeDrawable(UWindow *wnd, int shape)
	:UDrawable(wnd),
	mShape(shape)
{
	mWidth = -1;
	mHeight = -1;

	mHasSolid = false;
	mHasStroke = false;
	mRoundRadius = 1.f;
	mStrokeWidth = 1.f;
	mSolidColor = D2D1::ColorF(0, 0);
	mStrokeColor = D2D1::ColorF(0, 0);
}


UShapeDrawable::~UShapeDrawable()
{
}


void UShapeDrawable::setSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

void UShapeDrawable::setRadius(float radius)
{
	mRoundRadius = radius;
}

void UShapeDrawable::setSolidEnable(bool enable)
{
	mHasSolid = enable;
}

void UShapeDrawable::setSolidColor(D2D1_COLOR_F color)
{
	mSolidColor = color;
}

void UShapeDrawable::setStrokeEnable(bool enable)
{
	mHasStroke = enable;
}

void UShapeDrawable::setStrokeWidth(float width)
{
	mStrokeWidth = width;
}

void UShapeDrawable::setStrokeColor(D2D1_COLOR_F color)
{
	mStrokeColor = color;
}


void UShapeDrawable::draw(UCanvas *canvas)
{
	if (!mHasSolid && !mHasStroke)
		return;

	D2D1_RECT_F bound;
	if (mWidth == -1 || mHeight == -1)
		bound = getBound();
	else
		bound = D2D1::RectF(0, 0, mWidth, mHeight);

	switch (mShape)
	{
	case SHAPE_RECT:
	{
		if (mHasSolid)
			canvas->fillRect(bound, mSolidColor);
		if (mHasStroke)
			canvas->drawRect(bound, mStrokeColor);
		break;
	}
	case SHAPE_ROUND_RECT:
	{
		if (mHasSolid)
			canvas->fillRoundRect(bound, mRoundRadius, mSolidColor);
		if (mHasStroke)
			canvas->drawRoundRect(bound, mStrokeWidth, mRoundRadius, mStrokeColor);
		break;
	}
	case SHAPE_OVAL:
	{
		float cx = (bound.right - bound.left) / 2.f;
		float cy = (bound.bottom - bound.top) / 2.f;

		if (mHasSolid)
			canvas->fillOval(
				cx, cy, cx, cy, mSolidColor);
		if (mHasStroke)
			canvas->drawOval(
				cx, cy, cx, cy, mStrokeColor);
		break;
	}
	}
}

float UShapeDrawable::getOpacity()
{
	if ((mHasSolid && mSolidColor.a != 0.f)
		|| (mHasStroke && mStrokeColor.a != 0.f && mStrokeWidth != 0.f))
		return 1.f;
	else
		return 0.f;
}

int UShapeDrawable::getIncWidth()
{
	return mWidth;
}

int UShapeDrawable::getIncHeight()
{
	return mHeight;
}