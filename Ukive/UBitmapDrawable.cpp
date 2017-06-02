#include "UCommon.h"
#include "UCanvas.h"
#include "UBitmapDrawable.h"


UBitmapDrawable::UBitmapDrawable(UWindow *wnd, UComPtr<ID2D1Bitmap> bitmap)
	:UDrawable(wnd)
{
	if (bitmap == nullptr)
		throw std::invalid_argument("UBitmapDrawable-Construct(): null UBitmap* params.");

	mOpacity = 1.f;
	mBitmap = bitmap;
}


UBitmapDrawable::~UBitmapDrawable()
{
}


void UBitmapDrawable::setOpacity(float opt)
{
	if (opt < 0.f)
		opt = 0.f;

	if (opt > 1.f)
		opt = 1.f;

	mOpacity = opt;
}


void UBitmapDrawable::draw(UCanvas *canvas)
{
	canvas->drawBitmap(this->getBound(), mOpacity, mBitmap.get());
}

float UBitmapDrawable::getOpacity()
{
	return mOpacity;
}

int UBitmapDrawable::getIncWidth()
{
	D2D1_SIZE_F size = mBitmap->GetSize();
	return size.width;
}

int UBitmapDrawable::getIncHeight()
{
	D2D1_SIZE_F size = mBitmap->GetSize();
	return size.height;
}


UComPtr<ID2D1Bitmap> UBitmapDrawable::getBitmap()
{
	return mBitmap;
}