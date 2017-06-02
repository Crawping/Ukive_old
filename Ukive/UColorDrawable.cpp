#include "UCommon.h"
#include "UCanvas.h"
#include "UColorDrawable.h"


UColorDrawable::UColorDrawable(UWindow *wnd, D2D1_COLOR_F color)
	:UDrawable(wnd)
{
	mColor = color;
}


UColorDrawable::~UColorDrawable()
{
}


void UColorDrawable::draw(UCanvas *canvas)
{
	canvas->fillRect(this->getBound(), mColor);
}


float UColorDrawable::getOpacity()
{
	return mColor.a;
}


D2D1_COLOR_F UColorDrawable::getColor()
{
	return mColor;
}