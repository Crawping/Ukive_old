#include "UCommon.h"
#include "UCanvas.h"
#include "UColor.h"
#include "UEditTextDrawable.h"


UEditTextDrawable::UEditTextDrawable(UWindow *wnd)
	:UDrawable(wnd)
{
	mPanelLineWidth = 1;
	mPanelLineColor = UColor::Grey400;
}

UEditTextDrawable::~UEditTextDrawable()
{
}


bool UEditTextDrawable::onFocusChanged(bool focus)
{
	if (focus)
	{
		mPanelLineWidth = 2;
		mPanelLineColor = UColor::Blue500;
	}
	else
	{
		mPanelLineWidth = 1;
		mPanelLineColor = UColor::Grey400;
	}

	return true;
}


void UEditTextDrawable::draw(UCanvas *canvas)
{
	D2D1_RECT_F bound = getBound();

	canvas->fillRect(
		D2D1::RectF(
			bound.left, bound.bottom - 2,
			bound.right, bound.bottom - (2 - mPanelLineWidth)), 
		mPanelLineColor);
}

float UEditTextDrawable::getOpacity()
{
	return 1.f;
}