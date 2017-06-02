#include "UCommon.h"
#include "UMath.h"
#include "ULayoutParams.h"
#include "UFrameLayout.h"


UFrameLayout::UFrameLayout(UWindow *wnd)
	:UWidgetGroup(wnd)
{
}

UFrameLayout::UFrameLayout(UWindow *wnd, int id)
	: UWidgetGroup(wnd, id)
{
}

UFrameLayout::~UFrameLayout()
{
}


void UFrameLayout::onMeasure(int width, int height, int widthSpec, int heightSpec)
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


void UFrameLayout::onLayout(
	bool changed, bool sizeChanged,
	int left, int top, int right, int bottom)
{
	UWidget *widget;
	ULayoutParams *lp;

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		widget = getChildAt(i);
		if (widget->getVisibility() != UWidget::VANISHED)
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