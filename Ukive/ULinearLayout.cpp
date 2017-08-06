#include "UCommon.h"
#include "UMath.h"
#include "ULayoutParams.h"
#include "ULinearLayoutParams.h"
#include "ULinearLayout.h"


ULinearLayout::ULinearLayout(UWindow *wnd)
	:UWidgetGroup(wnd)
{
	mOrientation = VERTICAL;
}

ULinearLayout::ULinearLayout(UWindow *wnd, int id)
	:UWidgetGroup(wnd, id)
{
	mOrientation = VERTICAL;
}


ULinearLayout::~ULinearLayout()
{
}


ULayoutParams *ULinearLayout::generateLayoutParams(
	ULayoutParams *lp)
{
	return new ULinearLayoutParams(lp);
}

ULayoutParams *ULinearLayout::generateDefaultLayoutParams()
{
	return new ULinearLayoutParams(
		ULayoutParams::FIT_CONTENT,
		ULayoutParams::FIT_CONTENT);
}

bool ULinearLayout::checkLayoutParams(ULayoutParams *lp)
{
	return typeid(*lp) == typeid(ULinearLayoutParams);
}


void ULinearLayout::measureWeightedChildren(
	int totalWeight,
	int parentWidth, int parentHeight,
	int parentWidthMode, int parentHeightMode)
{
	for (unsigned int i = 0; i < this->getChildCount(); ++i)
	{
		UWidget *child = this->getChildAt(i);
		if (child->getVisibility() != UWidget::VANISHED)
		{
			ULinearLayoutParams *childParams = (ULinearLayoutParams*)child->getLayoutParams();

			int horizontalPadding = mPaddingLeft + mPaddingRight;
			int verticalPadding = mPaddingTop + mPaddingBottom;

			int horizontalMargins = childParams->leftMargin + childParams->rightMargin;
			int verticalMargins = childParams->topMargin + childParams->bottomMargin;

			int childWidth;
			int childWidthSpec;
			int childHeight;
			int childHeightSpec;

			this->getChildMeasure(
				parentWidth, parentWidthMode,
				horizontalMargins + horizontalPadding,
				childParams->width, &childWidth, &childWidthSpec);

			this->getChildMeasure(
				parentHeight, parentHeightMode,
				verticalMargins + verticalPadding,
				childParams->height, &childHeight, &childHeightSpec);

			if (mOrientation == VERTICAL)
			{
				childHeight = (int)UMath::round((childParams->weight / (float)totalWeight)*parentHeight);
				childHeight = UMath::maximum(0, childHeight - verticalMargins);
			}
			else if (mOrientation == HORIZONTAL)
			{
				childWidth = (int)UMath::round((childParams->weight / (float)totalWeight)*parentWidth);
				childWidth = UMath::maximum(0, childWidth - horizontalMargins);
			}

			child->measure(
				childWidth, childHeight,
				childWidthSpec, childHeightSpec);
		}
	}
}

void ULinearLayout::measureSequenceChildren(
	int parentWidth, int parentHeight,
	int parentWidthMode, int parentHeightMode)
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		if (child->getVisibility() != UWidget::VANISHED)
		{
			ULayoutParams *childParams = child->getLayoutParams();

			int horizontalPadding = mPaddingLeft + mPaddingRight;
			int verticalPadding = mPaddingTop + mPaddingBottom;

			int horizontalMargins = childParams->leftMargin + childParams->rightMargin;
			int verticalMargins = childParams->topMargin + childParams->bottomMargin;

			int childWidth;
			int childWidthSpec;
			int childHeight;
			int childHeightSpec;

			this->getChildMeasure(
				parentWidth, parentWidthMode,
				horizontalMargins + horizontalPadding,
				childParams->width, &childWidth, &childWidthSpec);

			this->getChildMeasure(
				parentHeight, parentHeightMode,
				verticalMargins + verticalPadding,
				childParams->height, &childHeight, &childHeightSpec);

			child->measure(childWidth, childHeight, childWidthSpec, childHeightSpec);
		}
	}
}

void ULinearLayout::measureLinearLayoutChildren(
	int parentWidth, int parentHeight,
	int parentWidthMode, int parentHeightMode)
{
	int totalWeight = 0;
	for (std::size_t i = 0; i < this->getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		if (child->getVisibility() != UWidget::VANISHED)
		{
			ULinearLayoutParams *lp = (ULinearLayoutParams*)child->getLayoutParams();
			totalWeight += lp->weight;
		}
	}

	if (totalWeight > 0)
		measureWeightedChildren(
			totalWeight, 
			parentWidth, parentHeight, 
			parentWidthMode, parentHeightMode);
	else
	{
		measureSequenceChildren(
			parentWidth, parentHeight,
			parentWidthMode, parentHeightMode);
	}
}


void ULinearLayout::measureVertical(int width, int height, int widthSpec, int heightSpec)
{
	int finalWidth = 0;
	int finalHeight = 0;

	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	measureLinearLayoutChildren(width, height, widthSpec, heightSpec);

	int totalHeight = 0;

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
	{
		UWidget *widget;
		ULayoutParams *lp;
		for (unsigned int i = 0; i < this->getChildCount(); ++i)
		{
			widget = this->getChildAt(i);
			if (widget->getVisibility() != UWidget::VANISHED)
			{
				lp = widget->getLayoutParams();
				totalHeight += widget->getMeasuredHeight() + lp->topMargin + lp->bottomMargin;
			}
		}
		finalHeight = UMath::minimum(height, totalHeight + verticalPadding);
		finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		break;
	}

	case UNKNOWN:
	{
		UWidget *widget;
		ULayoutParams *lp;
		for (unsigned int i = 0; i < this->getChildCount(); ++i)
		{
			widget = this->getChildAt(i);
			if (widget->getVisibility() != UWidget::VANISHED)
			{
				lp = widget->getLayoutParams();
				totalHeight += widget->getMeasuredHeight() + lp->topMargin + lp->bottomMargin;
			}
		}
		finalHeight = UMath::maximum(mMinimumHeight, totalHeight + verticalPadding);
		break;
	}

	case EXACTLY:
		finalHeight = height;
		break;
	}

	this->setMeasuredDimension(finalWidth, finalHeight);
}

void ULinearLayout::measureHorizontal(int width, int height, int widthSpec, int heightSpec)
{
	int finalWidth = 0;
	int finalHeight = 0;

	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	measureLinearLayoutChildren(width, height, widthSpec, heightSpec);

	int totalWidth = 0;

	switch (widthSpec)
	{
	case FIT:
	{
		UWidget *widget;
		ULayoutParams *lp;
		for (unsigned int i = 0; i < this->getChildCount(); ++i)
		{
			widget = this->getChildAt(i);
			if (widget->getVisibility() != UWidget::VANISHED)
			{
				lp = widget->getLayoutParams();
				totalWidth += widget->getMeasuredWidth() + lp->leftMargin + lp->rightMargin;
			}
		}
		finalWidth = UMath::minimum(totalWidth + horizontalPadding, width);
		finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		break;
	}

	case UNKNOWN:
	{
		UWidget *widget;
		ULayoutParams *lp;
		for (unsigned int i = 0; i < this->getChildCount(); ++i)
		{
			widget = this->getChildAt(i);
			if (widget->getVisibility() != UWidget::VANISHED)
			{
				lp = widget->getLayoutParams();
				totalWidth += widget->getMeasuredWidth() + lp->leftMargin + lp->rightMargin;
			}
		}
		finalWidth = UMath::maximum(mMinimumWidth, totalWidth + horizontalPadding);
		break;
	}

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


void ULinearLayout::layoutVertical(int left, int top, int right, int bottom)
{
	UWidget *widget = 0;
	ULayoutParams *lp = 0;

	int curTop = mPaddingTop;

	for (unsigned int i = 0; i < this->getChildCount(); ++i)
	{
		widget = this->getChildAt(i);
		if (widget->getVisibility() != UWidget::VANISHED)
		{
			lp = widget->getLayoutParams();

			int width = widget->getMeasuredWidth();
			int height = widget->getMeasuredHeight();

			curTop += lp->topMargin;

			widget->layout(
				mPaddingLeft + lp->leftMargin,
				curTop,
				mPaddingLeft + lp->leftMargin + width,
				curTop + height);

			curTop += height + lp->bottomMargin;
		}
	}
}

void ULinearLayout::layoutHorizontal(int left, int top, int right, int bottom)
{
	UWidget *widget = 0;
	ULayoutParams *lp = 0;

	int curLeft = mPaddingLeft;

	for (unsigned int i = 0; i < this->getChildCount(); ++i)
	{
		widget = this->getChildAt(i);
		if (widget->getVisibility() != UWidget::VANISHED)
		{
			lp = widget->getLayoutParams();

			int width = widget->getMeasuredWidth();
			int height = widget->getMeasuredHeight();

			curLeft += lp->leftMargin;

			widget->layout(
				curLeft,
				mPaddingTop + lp->topMargin,
				curLeft + width,
				mPaddingTop + lp->topMargin + height);

			curLeft += width + lp->rightMargin;
		}
	}
}


void ULinearLayout::onMeasure(int width, int height, int widthSpec, int heightSpec)
{
	if (mOrientation == VERTICAL)
		this->measureVertical(width, height, widthSpec, heightSpec);
	else
		this->measureHorizontal(width, height, widthSpec, heightSpec);
}


void ULinearLayout::onLayout(
	bool changed, bool sizeChanged, 
	int left, int top, int right, int bottom)
{
	if (mOrientation == VERTICAL)
		this->layoutVertical(left, top, right, bottom);
	else
		this->layoutHorizontal(left, top, right, bottom);
}


void ULinearLayout::setOrientation(int orientation)
{
	if (orientation == mOrientation)
		return;

	mOrientation = orientation;

	this->requestLayout();
	this->invalidate();
}