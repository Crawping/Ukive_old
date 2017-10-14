#include "UCommon.h"
#include "UWindow.h"
#include "UInputEvent.h"
#include "UCanvas.h"
#include "View.h"
#include "LayoutParams.h"
#include "BaseLayoutParams.h"
#include "BaseLayout.h"
#include "UDrawable.h"
#include "ViewAnimator.h"
#include "UInnerWindow.h"


UInnerWindow::UInnerWindow(UWindow *wnd)
{
	mParent = wnd;
	mDecorView = nullptr;
	mContentView = nullptr;
	mIsShowing = false;

	mElevation = 0.f;
	mOutsideTouchable = false;
	mDismissByTouchOutside = false;
	mBackgroundDrawable = nullptr;
	mWidth = LayoutParams::FIT_CONTENT;
	mHeight = LayoutParams::FIT_CONTENT;
}

UInnerWindow::~UInnerWindow()
{
	if (mDecorView)
		delete mDecorView;
}


void UInnerWindow::createDecorView()
{
	InnerDecorView *decorView
		= new InnerDecorView(this);
	decorView->addWidget(
		mContentView, new LayoutParams(mWidth, mHeight));

	decorView->setElevation(mElevation);
	decorView->setBackground(mBackgroundDrawable);
	decorView->setReceiveOutsideInputEvent(!mOutsideTouchable);

	mDecorView = decorView;
}


void UInnerWindow::setWidth(int width)
{
	mWidth = width;
}

void UInnerWindow::setHeight(int height)
{
	mHeight = height;
}

void UInnerWindow::setSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

void UInnerWindow::setElevation(float elevation)
{
	mElevation = elevation;
}

void UInnerWindow::setBackground(UDrawable *drawable)
{
	mBackgroundDrawable = drawable;
}

void UInnerWindow::setOutsideTouchable(bool touchable)
{
	mOutsideTouchable = touchable;
}

void UInnerWindow::setDismissByTouchOutside(bool enable)
{
	mDismissByTouchOutside = enable;
}

void UInnerWindow::setContentView(View *contentView)
{
	if (contentView == nullptr)
		throw std::invalid_argument("setContentView: null param");

	mContentView = contentView;
}


int UInnerWindow::getWidth()
{
	return mWidth;
}

int UInnerWindow::getHeight()
{
	return mHeight;
}

float UInnerWindow::getElevation()
{
	return mElevation;
}

UDrawable *UInnerWindow::getBackground()
{
	return mBackgroundDrawable;
}

bool UInnerWindow::isOutsideTouchable()
{
	return mOutsideTouchable;
}

bool UInnerWindow::isDismissByTouchOutside()
{
	return mDismissByTouchOutside;
}

UWindow *UInnerWindow::getParent()
{
	return mParent;
}

View *UInnerWindow::getContentView()
{
	return mContentView;
}

View *UInnerWindow::getDecorView()
{
	return mDecorView;
}

bool UInnerWindow::isShowing()
{
	return mIsShowing;
}


void UInnerWindow::show(int x, int y)
{
	if (mContentView == nullptr || mIsShowing)
		return;

	if (mDecorView)
		delete mDecorView;

	createDecorView();

	BaseLayoutParams *baselp
		= new BaseLayoutParams(mWidth, mHeight);
	baselp->leftMargin = x;
	baselp->topMargin = y;

	mDecorView->setLayoutParams(baselp);

	mParent->getBaseLayout()->addShade(mDecorView);

	mIsShowing = true;
}

void UInnerWindow::show(View *anchor, Gravity gravity)
{
	if (mContentView == nullptr 
		|| anchor == nullptr || mIsShowing)
		return;

	createDecorView();

	URect rect = anchor->getBoundInWindow();
}

void UInnerWindow::update(int x, int y)
{
	if (mDecorView == nullptr || !mIsShowing)
		return;

	BaseLayoutParams *baselp
		= (BaseLayoutParams*)mDecorView->getLayoutParams();
	baselp->leftMargin = x;
	baselp->topMargin = y;

	mDecorView->setLayoutParams(baselp);
}

void UInnerWindow::update(View *anchor, Gravity gravity)
{

}

void UInnerWindow::dismiss()
{
	if (mDecorView && mIsShowing)
		mParent->getBaseLayout()->removeShade(mDecorView);
	mIsShowing = false;
}


UInnerWindow::InnerDecorView::InnerDecorView(UInnerWindow *inner)
	:FrameLayout(inner->getParent())
{
	mInnerWindow = inner;
}

UInnerWindow::InnerDecorView::InnerDecorView(UInnerWindow *inner, int id)
	: FrameLayout(inner->getParent(), id)
{
	mInnerWindow = inner;
}

UInnerWindow::InnerDecorView::~InnerDecorView()
{
}


bool UInnerWindow::InnerDecorView::onInterceptInputEvent(UInputEvent *e)
{
	return false;
}

bool UInnerWindow::InnerDecorView::onInputEvent(UInputEvent *e)
{
	if (e->isOutside())
	{
		if (e->getEvent() == UInputEvent::EVENT_MOUSE_KEY_DOWN
			|| e->getEvent() == UInputEvent::EVENT_MOUSE_KEY_UP)
		{
			if (mInnerWindow->mDismissByTouchOutside)
				mInnerWindow->dismiss();
		}
		return true;
	}

	return false;
}