#include "UCommon.h"
#include "UCanvas.h"
#include "UDrawable.h"


UDrawable::UDrawable(UWindow *wnd)
{
	if (wnd == nullptr)
		throw std::logic_error("UDrawable-Constructor: null UWindow* param.");

	mStartX = 0.f;
	mStartY = 0.f;
	mParentHasFocus = false;

	mState = STATE_NONE;
	mPrevState = STATE_NONE;
	mOwnerWindow = wnd;
}


UDrawable::~UDrawable()
{
}


void UDrawable::setBound(D2D1_RECT_F &rect)
{
	D2D1_RECT_F oldRect = mRect;
	if (oldRect.left == rect.left
		&& oldRect.top == rect.top
		&& oldRect.right == rect.right
		&& oldRect.bottom == rect.bottom)
		return;

	mRect = rect;
	this->onBoundChanged(mRect);
}

void UDrawable::setBound(int left, int top, int right, int bottom)
{
	D2D1_RECT_F oldRect = mRect;
	if (oldRect.left == left
		&& oldRect.top == top
		&& oldRect.right == right
		&& oldRect.bottom == bottom)
		return;

	mRect.left = left;
	mRect.top = top;
	mRect.right = right;
	mRect.bottom = bottom;

	this->onBoundChanged(mRect);
}

bool UDrawable::setState(int state)
{
	if (state == mState)
		return false;

	mPrevState = mState;
	mState = state;

	return onStateChanged(state, mPrevState);
}

void UDrawable::setHotspot(int x, int y)
{
	mStartX = x;
	mStartY = y;
}

bool UDrawable::setParentFocus(bool focus)
{
	if (focus == mParentHasFocus)
		return false;

	mParentHasFocus = focus;
	return onFocusChanged(mParentHasFocus);
}


int UDrawable::getState()
{
	return mState;
}

int UDrawable::getPrevState()
{
	return mPrevState;
}

D2D1_RECT_F &UDrawable::getBound()
{
	return mRect;
}

float UDrawable::getOpacity()
{
	return 1.f;
}


bool UDrawable::onFocusChanged(bool focus)
{
	return false;
}

void UDrawable::onBoundChanged(D2D1_RECT_F &newBound)
{
}

bool UDrawable::onStateChanged(int newState, int prevState)
{
	return false;
}


int UDrawable::getIncWidth()
{
	return -1;
}

int UDrawable::getIncHeight()
{
	return -1;
}