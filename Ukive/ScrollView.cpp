#include "UCommon.h"
#include "UMath.h"
#include "UWindow.h"
#include "UScroller.h"
#include "UInputEvent.h"
#include "LayoutParams.h"
#include "ScrollView.h"


ScrollView::ScrollView(UWindow *wnd)
	:ViewGroup(wnd)
{
}

ScrollView::ScrollView(UWindow *wnd, int id)
	: ViewGroup(wnd, id)
{
}

ScrollView::~ScrollView()
{
}


bool ScrollView::canScroll()
{
	View *widget = getChildAt(0);
	if (widget && widget->getVisibility() != View::VANISHED)
	{
		if (getMeasuredHeight() < widget->getMeasuredHeight())
			return true;
	}

	return false;
}

int ScrollView::computeScrollRange()
{
	View *widget = getChildAt(0);
	if (widget && widget->getVisibility() != View::VANISHED)
		return widget->getMeasuredHeight();
	return 0;
}

int ScrollView::computeScrollExtend()
{
	View *widget = getChildAt(0);
	if (widget && widget->getVisibility() != View::VANISHED)
		return (widget->getMeasuredHeight() - getMeasuredHeight());
	return 0;
}

void ScrollView::processVerticalScroll(int dy)
{
	int finalDy = 0;
	if (dy > 0)
	{
		int scrollY = getScrollY();
		finalDy = UMath::maximum(scrollY - dy, 0) - scrollY;
	}
	else if (dy < 0)
	{
		int scrollY = getScrollY();
		int extend = this->computeScrollExtend();
		if (extend > 0)
			finalDy = UMath::minimum(scrollY - dy, extend) - scrollY;
	}

	if (finalDy != 0)
		this->scrollBy(0, finalDy);
}


void ScrollView::onMeasure(
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
		View *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != View::VANISHED)
		{
			int childWidth = widget->getMeasuredWidth();
			finalWidth = UMath::minimum(childWidth + horizontalPadding, width);
			finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		}
		break;
	}

	case UNKNOWN:
	{
		View *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != View::VANISHED)
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
		View *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != View::VANISHED)
		{
			int childHeight = widget->getMeasuredHeight();
			finalHeight = UMath::minimum(childHeight + verticalPadding, height);
			finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		}
		break;
	}

	case UNKNOWN:
	{
		View *widget = this->getChildAt(0);
		if (widget && widget->getVisibility() != View::VANISHED)
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

void ScrollView::onLayout(
	bool changed, bool sizeChanged,
	int left, int top, int right, int bottom)
{
	View *widget = this->getChildAt(0);
	if (widget && widget->getVisibility() != View::VANISHED)
	{
		LayoutParams *lp = widget->getLayoutParams();

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

void ScrollView::onSizeChanged(
	int width, int height, int oldWidth, int oldHeight)
{
	if (width > 0 && height > 0
		&& oldWidth > 0 && oldHeight > 0)
	{
		int changed = 0;

		if (canScroll())
		{
			if (getScrollY() < 0)
				changed = -getScrollY();

			int extend = this->computeScrollExtend();
			if (getScrollY() > extend)
				changed = extend - getScrollY();
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

void ScrollView::onScrollChanged(
	int scrollX, int scrollY, int oldScrollX, int oldScrollY)
{
	ViewGroup::onScrollChanged(scrollX, scrollY, oldScrollX, oldScrollY);

	UInputEvent e;
	e.setEvent(UInputEvent::EVENT_MOUSE_SCROLL_ENTER);
	e.setMouseX(mMouseXCache + mLeft - mScrollX - (oldScrollX - scrollX));
	e.setMouseY(mMouseYCache + mTop - mScrollY - (oldScrollY - scrollY));
	this->dispatchInputEvent(&e);
}


bool ScrollView::onInputEvent(UInputEvent *e)
{
	ViewGroup::onInputEvent(e);

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

bool ScrollView::onInterceptMouseEvent(UInputEvent *e)
{
	return false;
}