#include "UCommon.h"
#include "UMath.h"
#include "UWindow.h"
#include "UScroller.h"
#include "UInputEvent.h"
#include "ULayoutParams.h"
#include "UScrollView.h"


UScrollView::UScrollView(UWindow *wnd)
	:UWidgetGroup(wnd)
{
}

UScrollView::UScrollView(UWindow *wnd, int id)
	:UWidgetGroup(wnd, id)
{
}

UScrollView::~UScrollView()
{
}


bool UScrollView::canScroll()
{
	UWidget *widget = getChildAt(0);
	if (widget && widget->getVisibility() != UWidget::VANISHED)
	{
		if (getMeasuredHeight() < widget->getMeasuredHeight())
			return true;
	}

	return false;
}

int UScrollView::computeScrollRange()
{
	UWidget *widget = getChildAt(0);
	if (widget && widget->getVisibility() != UWidget::VANISHED)
		return widget->getMeasuredHeight();
	return 0;
}

int UScrollView::computeScrollExtend()
{
	UWidget *widget = getChildAt(0);
	if (widget && widget->getVisibility() != UWidget::VANISHED)
		return (widget->getMeasuredHeight() - getMeasuredHeight());
	return 0;
}

void UScrollView::processVerticalScroll(int dy)
{
	int finalDy = 0;
	int scrollY = getScrollY();
	if (dy > 0)
	{
		finalDy = UMath::minimum(scrollY + dy, 0) - scrollY;
	}
	else if (dy < 0)
	{
		int extend = this->computeScrollExtend();
		finalDy = UMath::maximum(scrollY + dy, -extend) - scrollY;
	}

	if (finalDy != 0)
		this->scrollBy(0, finalDy);
}


void UScrollView::onMeasure(
	int width, int height,
	int widthSpec, int heightSpec)
{
	int finalWidth = 0;
	int finalHeight = 0;

	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	if (getChildCount() > 1)
		throw std::logic_error("UScrollView-onMeasure(): UScrollView can only have one child.");

	this->measureChildrenWithMargins(width, 0, widthSpec, UNKNOWN);

	switch (widthSpec)
	{
	case FIT:
	{
		UWidget *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != UWidget::VANISHED)
		{
			int childWidth = widget->getMeasuredWidth();
			finalWidth = UMath::minimum(childWidth + horizontalPadding, width);
			finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		}
		break;
	}

	case UNKNOWN:
	{
		UWidget *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != UWidget::VANISHED)
		{
			int childWidth = widget->getMeasuredWidth();
			finalWidth = UMath::maximum(mMinimumWidth, childWidth);
		}
		break;
	}

	case EXACTLY:
		finalWidth = width;
		break;
	}

	switch (heightSpec)
	{
	case FIT:
	{
		UWidget *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != UWidget::VANISHED)
		{
			int childHeight = widget->getMeasuredHeight();
			finalHeight = UMath::minimum(childHeight + verticalPadding, height);
			finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		}
		break;
	}

	case UNKNOWN:
	{
		UWidget *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != UWidget::VANISHED)
		{
			int childHeight = widget->getMeasuredHeight();
			finalHeight = UMath::maximum(mMinimumHeight, childHeight);
		}
		break;
	}

	case EXACTLY:
		finalHeight = height;
		break;
	}

	this->setMeasuredDimension(finalWidth, finalHeight);
}

void UScrollView::onLayout(
	bool changed, bool sizeChanged,
	int left, int top, int right, int bottom)
{
	UWidget *widget = this->getChildAt(0);
	if (widget && widget->getVisibility() != UWidget::VANISHED)
	{
		ULayoutParams *lp = widget->getLayoutParams();

		int width = widget->getMeasuredWidth();
		int height = widget->getMeasuredHeight();

		int childleft = mPaddingLeft + lp->leftMargin;
		int childTop = mPaddingTop + lp->topMargin;

		widget->layout(
			childleft,
			childTop,
			width + childleft,
			height + childTop);
	}
}

void UScrollView::onSizeChanged(
	int width, int height, int oldWidth, int oldHeight)
{
	if (width > 0 && height > 0
		&& oldWidth > 0 && oldHeight > 0)
	{
		int changed = 0;

		if (canScroll())
		{
			if (getScrollY() > 0)
				changed = -getScrollY();

			int extend = this->computeScrollExtend();
			if (getScrollY() < -extend)
				changed = UMath::maximum(getScrollY(), -extend) - getScrollY();
		}
		else
		{
			if (getScrollY() != 0)
				changed = -getScrollY();
		}

		if (changed != 0)
			scrollBy(0, changed);
	}
}

void UScrollView::onScrollChanged(
	int scrollX, int scrollY, int oldScrollX, int oldScrollY)
{
	UWidgetGroup::onScrollChanged(scrollX, scrollY, oldScrollX, oldScrollY);

	UInputEvent e;
	e.setEvent(UInputEvent::EVENT_MOUSE_SCROLL_ENTER);
	e.setMouseX(mMouseXCache + mLeft + mScrollX - (scrollX - oldScrollX));
	e.setMouseY(mMouseYCache + mTop + mScrollY - (scrollY - oldScrollY));
	this->dispatchInputEvent(&e);
}


bool UScrollView::onInputEvent(UInputEvent *e)
{
	UWidgetGroup::onInputEvent(e);

	switch (e->getEvent())
	{
	case UInputEvent::EVENT_MOUSE_WHEEL:
		mMouseXCache = e->getMouseX();
		mMouseYCache = e->getMouseY();
		processVerticalScroll(30 * e->getMouseWheel());
		return true;
	}

	return false;
}

bool UScrollView::onInterceptMouseEvent(UInputEvent *e)
{
	return false;
}