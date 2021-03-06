﻿#include "UCommon.h"
#include "UMath.h"
#include "UCanvas.h"
#include "UInputEvent.h"
#include "LayoutParams.h"
#include "UMenuImpl.h"
#include "ViewGroup.h"


ViewGroup::ViewGroup(UWindow *wnd)
	:View(wnd)
{
	initViewGroup();
}

ViewGroup::ViewGroup(UWindow *wnd, int id)
	:View(wnd, id)
{
	initViewGroup();
}


ViewGroup::~ViewGroup()
{
}


void ViewGroup::initViewGroup()
{
}


LayoutParams *ViewGroup::generateLayoutParams(
	LayoutParams *lp)
{
	return new LayoutParams(lp);
}

LayoutParams *ViewGroup::generateDefaultLayoutParams()
{
	return new LayoutParams(
		LayoutParams::FIT_CONTENT,
		LayoutParams::FIT_CONTENT);
}

bool ViewGroup::checkLayoutParams(LayoutParams *lp)
{
	return lp != nullptr;
}


int ViewGroup::getWrappedWidth()
{
	int wrappedWidth = 0;
	for (std::size_t i = 0; i < this->getChildCount(); ++i)
	{
		View *widget = this->getChildAt(i);
		if (widget->getVisibility() != View::VANISHED)
		{
			auto lp = widget->getLayoutParams();

			int childWidth = widget->getMeasuredWidth() + lp->leftMargin + lp->rightMargin;
			if (childWidth > wrappedWidth)
				wrappedWidth = childWidth;
		}
	}

	return wrappedWidth;
}

int ViewGroup::getWrappedHeight()
{
	int wrappedHeight = 0;
	for (std::size_t i = 0; i < this->getChildCount(); ++i)
	{
		View *widget = this->getChildAt(i);
		if (widget->getVisibility() != View::VANISHED)
		{
			auto lp = widget->getLayoutParams();

			int childHeight = widget->getMeasuredHeight() + lp->topMargin + lp->bottomMargin;
			if (childHeight > wrappedHeight)
				wrappedHeight = childHeight;
		}
	}

	return wrappedHeight;
}


void ViewGroup::onAttachedToWindow()
{
	View::onAttachedToWindow();

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if (!(*it)->isAttachedToWindow())
			(*it)->onAttachedToWindow();
	}
}

void ViewGroup::onDetachedFromWindow()
{
	View::onDetachedFromWindow();

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->isAttachedToWindow())
			(*it)->onDetachedFromWindow();
	}
}


void ViewGroup::addWidget(View *widget)
{
	this->addWidget(widget, nullptr);
}

void ViewGroup::addWidget(View *widget, LayoutParams *params)
{
	this->addWidget(mWidgetList.size(), widget, params);
}

void ViewGroup::addWidget(std::size_t index, View *widget, LayoutParams *params)
{
	if (widget == nullptr)
		throw std::invalid_argument(
			"UWidgetGroup-addWidget(): You cannot add a null Widget to UWidgetGroup.");
	if (index > mWidgetList.size())
		throw std::invalid_argument(
			"UWidgetGroup-addWidget(): invalid index");

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->getId() == widget->getId())
			return;
	}

	if (params == nullptr)
	{
		params = widget->getLayoutParams();
		if (params == nullptr)
		{
			params = this->generateDefaultLayoutParams();
			if (params == nullptr)
				throw std::logic_error(
					"UWidgetGroup-addWidget(): You cannot add a null Widget to UWidgetGroup.");
		}
	}

	if (!checkLayoutParams(params))
		params = this->generateLayoutParams(params);

	widget->setParent(this);
	widget->setLayoutParams(params);

	if (index == mWidgetList.size())
		mWidgetList.push_back(std::shared_ptr<View>(widget));
	else
	{
		mWidgetList.insert(
			mWidgetList.begin() + index,
			std::shared_ptr<View>(widget));
	}

	if (!widget->isAttachedToWindow() && mIsAttachdToWindow)
		widget->onAttachedToWindow();

	this->requestLayout();
	this->invalidate();
}

void ViewGroup::addWidget(std::shared_ptr<View> widget)
{
	this->addWidget(widget, nullptr);
}

void ViewGroup::addWidget(std::shared_ptr<View> widget, LayoutParams *params)
{
	this->addWidget(mWidgetList.size(), widget, params);
}

void ViewGroup::addWidget(std::size_t index, std::shared_ptr<View> widget, LayoutParams *params)
{
	if (widget == nullptr)
		throw std::invalid_argument(
			"UWidgetGroup-addWidget(): You cannot add a null Widget to UWidgetGroup.");
	if (index > mWidgetList.size())
		throw std::invalid_argument(
			"UWidgetGroup-addWidget(): invalid index");

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->getId() == widget->getId())
			return;
	}

	if (params == nullptr)
	{
		params = widget->getLayoutParams();
		if (params == nullptr)
		{
			params = this->generateDefaultLayoutParams();
			if (params == nullptr)
				throw std::logic_error(
					"UWidgetGroup-addWidget(): You cannot add a null Widget to UWidgetGroup.");
		}
	}

	if (!checkLayoutParams(params))
		params = this->generateLayoutParams(params);

	widget->setParent(this);
	widget->setLayoutParams(params);

	if (index == mWidgetList.size())
		mWidgetList.push_back(widget);
	else
	{
		mWidgetList.insert(
			mWidgetList.begin() + index,
			widget);
	}

	if (!widget->isAttachedToWindow() && mIsAttachdToWindow)
		widget->onAttachedToWindow();

	this->requestLayout();
	this->invalidate();
}


void ViewGroup::removeWidget(View *widget)
{
	if (widget == nullptr)
		throw std::invalid_argument(
			"UWidgetGroup-removeWidget(): You cannot remove a null Widget from UWidgetGroup.");

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->getId() == widget->getId())
		{
			widget->discardFocus();
			widget->discardPendingOperations();

			if (widget->isAttachedToWindow() && mIsAttachdToWindow)
				widget->onDetachedFromWindow();

			mWidgetList.erase(it);

			this->requestLayout();
			this->invalidate();
			return;
		}
	}
}

void ViewGroup::removeWidget(std::shared_ptr<View> widget)
{
	if (widget == nullptr)
		throw std::invalid_argument(
			"UWidgetGroup-removeWidget(): You cannot remove a null Widget from UWidgetGroup.");

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->getId() == widget->getId())
		{
			widget->discardFocus();
			widget->discardPendingOperations();

			if (widget->isAttachedToWindow() && mIsAttachdToWindow)
				widget->onDetachedFromWindow();

			mWidgetList.erase(it);

			this->requestLayout();
			this->invalidate();
			return;
		}
	}
}

void ViewGroup::removeWidget(std::size_t index)
{
	View *widget = mWidgetList.at(index).get();

	widget->discardFocus();
	widget->discardPendingOperations();

	if (widget->isAttachedToWindow() && mIsAttachdToWindow)
		widget->onDetachedFromWindow();

	mWidgetList.erase(mWidgetList.begin() + index);

	this->requestLayout();
	this->invalidate();
}


View *ViewGroup::findWidgetById(int id)
{
	std::queue<View*> curQueue;
	std::queue<View*> nextQueue;

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->getId() == id)
			return (*it).get();
		else
		{
			View *widget = (*it).get()->findWidgetById(id);
			if (widget)
				return widget;
		}
	}

	return nullptr;
}


std::size_t ViewGroup::getChildCount()
{
	return mWidgetList.size();
}

View *ViewGroup::getChildById(int id)
{
	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it)->getId() == id)
			return (*it).get();
	}

	return 0;
}

View *ViewGroup::getChildAt(std::size_t index)
{
	return mWidgetList.at(index).get();
}


void ViewGroup::dispatchDraw(UCanvas *canvas)
{
	this->drawChildren(canvas);
}

void ViewGroup::dispatchDiscardFocus()
{
	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		(*it)->discardFocus();
	}
}

void ViewGroup::dispatchDiscardPendingOperations()
{
	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		(*it)->discardPendingOperations();
	}
}


bool ViewGroup::dispatchMouseEvent(UInputEvent *e)
{
	bool consumed = false;
	bool isIntercepted = false;

	e->setMouseX(e->getMouseX() - mLeft + mScrollX);
	e->setMouseY(e->getMouseY() - mTop + mScrollY);

	if (this->onInterceptInputEvent(e)
		|| this->onInterceptMouseEvent(e))
	{
		consumed = this->onInputEvent(e);
		isIntercepted = true;
		e->setEvent(UInputEvent::EVENT_CANCEL);
	}

	//从Widget列表的尾部开始遍历。因为最近添加的Widget在列表尾部，
	//而最近添加的Widget可能会处于其他之前添加的Widget的上面（在绘制
	//Widget是从列表头开始的），这样一来与坐标相关的事件就可能发生在
	//多个Widget交叠的区域，此时应该将该事件先发送至最上层的Widget，
	//为此从Widget列表的尾部开始遍历。
	//随后根据子Widget的dispatchInputEvent()方法的返回值来决定是否将
	//该事件传递给下层的Widget。
	for (auto it = mWidgetList.rbegin();
		it != mWidgetList.rend();
		++it)
	{
		View *child = (*it).get();
		if (child->getVisibility() != View::VISIBLE
			|| !child->isEnabled())
			continue;

		int mx = e->getMouseX();
		int my = e->getMouseY();

		if (child->isGroupMouseInThis(e)
			&& e->getEvent() != UInputEvent::EVENT_MOUSE_LEAVE_WINDOW
			&& e->getEvent() != UInputEvent::EVENT_CANCEL)
		{
			if (!consumed)
			{
				consumed = child->dispatchInputEvent(e);
				child->setIsInputEventAtLast(true);
			}
		}
		else if (child->isReceiveOutsideInputEvent()
			&& e->getEvent() != UInputEvent::EVENT_MOUSE_LEAVE_WINDOW
			&& e->getEvent() != UInputEvent::EVENT_CANCEL)
		{
			if (!consumed)
			{
				e->setOutside(true);

				consumed = child->dispatchInputEvent(e);
				child->setIsInputEventAtLast(false);

				e->setOutside(false);
			}
		}
		else
		{
			if (child->isInputEventAtLast())
			{
				int savedEvent = e->getEvent();

				if (savedEvent == UInputEvent::EVENT_CANCEL)
					e->setEvent(UInputEvent::EVENT_CANCEL);
				else
					e->setEvent(UInputEvent::EVENT_MOUSE_LEAVE);

				child->dispatchInputEvent(e);
				child->setIsInputEventAtLast(false);
				e->setEvent(savedEvent);
			}
		}

		e->setMouseX(mx);
		e->setMouseY(my);
	}

	if(!isIntercepted && !consumed)
		consumed = this->onInputEvent(e);

	return consumed;
}

bool ViewGroup::dispatchKeyboardEvent(UInputEvent *e)
{
	if (this->onInterceptInputEvent(e))
		return this->onInputEvent(e);

	if (this->onInterceptKeyboardEvent(e))
		return this->onInputEvent(e);

	//键盘事件不参与分发。
	return false;
}

bool ViewGroup::dispatchInputEvent(UInputEvent *e)
{
	bool consumed = false;

	if (e->isMouseEvent() && !e->isMouseCaptured())
	{
		consumed = this->dispatchMouseEvent(e);
	}
	else if (e->isKeyboardEvent())
	{
		consumed = this->dispatchKeyboardEvent(e);
	}
	else
	{
		consumed = this->onInputEvent(e);
	}

	return consumed;
}

void ViewGroup::dispatchWindowFocusChanged(bool windowFocus)
{
	this->onWindowFocusChanged(windowFocus);

	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		(*it)->dispatchWindowFocusChanged(windowFocus);
	}
}

bool ViewGroup::onInterceptInputEvent(UInputEvent *e)
{
	return false;
}

bool ViewGroup::onInterceptMouseEvent(UInputEvent *e)
{
	return false;
}

bool ViewGroup::onInterceptKeyboardEvent(UInputEvent *e)
{
	return false;
}


void ViewGroup::drawChild(UCanvas *canvas, View *child)
{
	if (child->isLayouted()
		&& child->getVisibility() == View::VISIBLE
		&& child->getWidth() > 0 && child->getHeight() > 0)
	{
		canvas->save();
		canvas->translate(child->getLeft(), child->getTop());
		child->draw(canvas);
		canvas->restore();
	}
}

void ViewGroup::drawChildren(UCanvas *canvas)
{
	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		this->drawChild(canvas, (*it).get());
	}
}


void ViewGroup::getChildMeasure(
	int parentSize, int parentSizeMode,
	int padding, int childDimension,
	int *childSize, int *childSizeMode)
{
	int size = UMath::maximum(0, parentSize - padding);

	int resultSize = 0;
	int resultSpec = 0;

	switch (parentSizeMode)
	{
		//父ViewGroup以其子View决定大小。
		//此时，若子View的大小未给出，应测量自身。
	case FIT:
		if (childDimension >= 0)
		{
			resultSize = childDimension;
			resultSpec = EXACTLY;
		}
		else if (childDimension == LayoutParams::MATCH_PARENT)
		{
			resultSize = size;
			resultSpec = FIT;
		}
		else if (childDimension == LayoutParams::FIT_CONTENT)
		{
			resultSize = size;
			resultSpec = FIT;
		}
		break;

		//父ViewGroup的大小已确定。
		//此时，子View的度量模式不变。
	case EXACTLY:
		if (childDimension >= 0)
		{
			resultSize = childDimension;
			resultSpec = EXACTLY;
		}
		else if (childDimension == LayoutParams::MATCH_PARENT)
		{
			resultSize = size;
			resultSpec = EXACTLY;
		}
		else if (childDimension == LayoutParams::FIT_CONTENT)
		{
			resultSize = size;
			resultSpec = FIT;
		}
		break;

		//父ViewGroup的大小未知。
		//此时，除非子View的大小已给出，否则子View的度量模式也为未知。
	case UNKNOWN:
		if (childDimension >= 0)
		{
			resultSize = childDimension;
			resultSpec = EXACTLY;
		}
		else if (childDimension == LayoutParams::MATCH_PARENT)
		{
			resultSize = size;
			resultSpec = UNKNOWN;
		}
		else if (childDimension == LayoutParams::FIT_CONTENT)
		{
			resultSize = size;
			resultSpec = UNKNOWN;
		}
		break;
	}

	*childSize = resultSize;
	*childSizeMode = resultSpec;
}


void ViewGroup::measureChild(
	View *child, 
	int parentWidth, int parentHeight, 
	int parentWidthMode, int parentHeightMode)
{
	LayoutParams *childParams = child->getLayoutParams();

	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	int childWidth;
	int childWidthSpec;
	int childHeight;
	int childHeightSpec;

	this->getChildMeasure(
		parentWidth, parentWidthMode,
		horizontalPadding,
		childParams->width, &childWidth, &childWidthSpec);

	this->getChildMeasure(
		parentHeight, parentHeightMode,
		verticalPadding,
		childParams->height, &childHeight, &childHeightSpec);

	child->measure(childWidth, childHeight, childWidthSpec, childHeightSpec);
}


void ViewGroup::measureChildWithMargins(
	View *child,
	int parentWidth, int parentHeight,
	int parentWidthMode, int parentHeightMode)
{
	LayoutParams *childParams = child->getLayoutParams();

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


void ViewGroup::measureChildren(
	int parentWidth, int parentHeight, 
	int parentWidthMode, int parentHeightMode)
{
	for (auto it = mWidgetList.begin();
		it != mWidgetList.end();
		++it)
	{
		if ((*it).get()->getVisibility() != View::VANISHED)
		{
			this->measureChild(
				(*it).get(), parentWidth, parentHeight, parentWidthMode, parentHeightMode);
		}
	}
}

void ViewGroup::measureChildrenWithMargins(
	int parentWidth, int parentHeight,
	int parentWidthMode, int parentHeightMode)
{
	for (auto it = mWidgetList.begin();
		it != mWidgetList.end(); ++it)
	{
		if ((*it).get()->getVisibility() != View::VANISHED)
		{
			this->measureChildWithMargins(
				(*it).get(), parentWidth, parentHeight, parentWidthMode, parentHeightMode);
		}
	}
}