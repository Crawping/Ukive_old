#include "UCommon.h"
#include "UMath.h"
#include "UBitmapDrawable.h"
#include "ImageView.h"


ImageView::ImageView(UWindow *wnd)
	:View(wnd)
{
	mBitmapDrawable = 0;
}

ImageView::ImageView(UWindow *wnd, int id)
	: View(wnd, id)
{
	mBitmapDrawable = 0;
}

ImageView::~ImageView()
{
}


void ImageView::onMeasure(int width, int height, int widthSpec, int heightSpec)
{
	int finalWidth = 0;
	int finalHeight = 0;

	int verticalPadding = mPaddingTop + mPaddingBottom;
	int horizontalPadding = mPaddingLeft + mPaddingRight;

	switch (widthSpec)
	{
	case FIT:
		if (mBitmapDrawable != 0)
			finalWidth = mBitmapDrawable->getIncWidth();

		finalWidth = UMath::maximum(mMinimumWidth, finalWidth + horizontalPadding);
		finalWidth = UMath::minimum(width, finalWidth);
		break;

	case UNKNOWN:
		if (mBitmapDrawable != 0)
			finalWidth = mBitmapDrawable->getIncWidth();

		finalWidth = UMath::maximum(mMinimumWidth, finalWidth + horizontalPadding);
		break;

	case EXACTLY:
		finalWidth = width;
		break;
	}

	switch (heightSpec)
	{
	case FIT:
		if (mBitmapDrawable != 0)
			finalHeight = mBitmapDrawable->getIncHeight();

		finalHeight = UMath::maximum(mMinimumHeight, finalHeight + verticalPadding);
		finalHeight = UMath::minimum(height, finalHeight);
		break;

	case UNKNOWN:
		if (mBitmapDrawable != 0)
			finalHeight = mBitmapDrawable->getIncHeight();

		finalHeight = UMath::maximum(mMinimumHeight, finalHeight + verticalPadding);
		break;

	case EXACTLY:
		finalHeight = height;
		break;
	}

	setMeasuredDimension(finalWidth, finalHeight);
}

void ImageView::onDraw(UCanvas *canvas)
{
	View::onDraw(canvas);

	if (mBitmapDrawable)
		mBitmapDrawable->draw(canvas);
}

bool ImageView::onInputEvent(UInputEvent *e)
{
	View::onInputEvent(e);

	return false;
}

void ImageView::onSizeChanged(int width, int height, int oldWidth, int oldHeight)
{
}


void ImageView::setImageBitmap(UComPtr<ID2D1Bitmap> bitmap)
{
	if (bitmap != nullptr)
	{
		D2D1_SIZE_F size = bitmap->GetSize();

		mBitmapDrawable = new UBitmapDrawable(mWindow, bitmap);
		mBitmapDrawable->setBound(0, 0, size.width, size.height);
	}
}