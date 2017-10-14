#include "UCommon.h"
#include "UMath.h"
#include "LayoutParams.h"
#include "FrameLayout.h"


FrameLayout::FrameLayout(UWindow *wnd)
	:ViewGroup(wnd)
{
}

FrameLayout::FrameLayout(UWindow *wnd, int id)
	: ViewGroup(wnd, id)
{
}

FrameLayout::~FrameLayout()
{
}


void FrameLayout::onMeasure(int width, int height, int widthSpec, int heightSpec)
{
	int finalWidth = 0;
	int finalHeight = 0;

	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	this->measureChildrenWithMargins(width, height, widthSpec, heightSpec);

	switch (widthSpec)
	{
	case FIT:
		finalWidth = getWrappedWidth();
		finalWidth = UMath::minimum(finalWidth + horizontalPadding, width);
		finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		break;

	case UNKNOWN:
		finalWidth = getWrappedWidth();
		finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		break;

	case EXACTLY:
		finalWidth = width;
		break;
	}

	switch (heightSpec)
	{
	case FIT:
		finalHeight = getWrappedHeight();
		finalHeight = UMath::minimum(finalHeight + verticalPadding, height);
		finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		break;

	case UNKNOWN:
		finalHeight = getWrappedHeight();
		finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		break;

	case EXACTLY:
		finalHeight = height;
		break;
	}

	this->setMeasuredDimension(finalWidth, finalHeight);
}


void FrameLayout::onLayout(
	bool changed, bool sizeChanged,
	int left, int top, int right, int bottom)
{
	View *widget;
	LayoutParams *lp;

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		widget = getChildAt(i);
		if (widget->getVisibility() != View::VANISHED)
		{
			lp = widget->getLayoutParams();

			int width = widget->getMeasuredWidth();
			int height = widget->getMeasuredHeight();

			int left = mPaddingLeft + lp->leftMargin;
			int top = mPaddingTop + lp->topMargin;

			widget->layout(
				left,
				top,
				width + left,
				height + top);
		}
	}
}