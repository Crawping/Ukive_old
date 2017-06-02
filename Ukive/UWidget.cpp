#include "UCommon.h"
#include "UWindow.h"
#include "UInputEvent.h"
#include "UCanvas.h"
#include "UDrawable.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "UTsfManager.h"
#include "UInputConnection.h"
#include "URenderer.h"
#include "ULayoutParams.h"
#include "OnClickListener.h"
#include "UWidgetAnimator.h"
#include "UInnerWindow.h"
#include "UDeviceManager.h"
#include "UWidget.h"


UWidget::UWidget(UWindow *wnd)
	:UWidget(wnd, UApplication::getWidgetUID())
{
}

UWidget::UWidget(UWindow *wnd, int id)
{
	if (wnd == nullptr)
		throw std::invalid_argument(
			"UWidget-Constructor(): null UWindow* param");

	mAlpha = 1.f;
	mScaleX = 1.f;
	mScaleY = 1.f;
	mTranslateX = 0;
	mTranslateY = 0;
	mPivotX = mPivotY = 0;

	mRevealType = UWidgetAnimator::REVEAL_CIRCULE;
	mHasReveal = false;
	mRevealRadius = 0.f;
	mRevealCenterX = mRevealCenterY = 0.f;
	mRevealWidthRadius = mRevealHeightRadius = 0.f;

	mId = id;
	mLeft = mRight = mTop = mBottom = 0;
	mMeasuredWidth = mMeasuredHeight = 0;
	mMinimumWidth = mMinimumHeight = 0;

	mScrollX = mScrollY = 0;
	mPaddingLeft = mPaddingRight = mPaddingTop = mPaddingBottom = 0;

	mElevation = 0.f;

	mVisibility = VISIBLE;
	mIsEnable = true;
	mIsAttachdToWindow = false;
	mIsInputEventAtLast = false;
	mIsPressed = false;
	mHasFocus = false;
	mIsFocusable = false;
	mIsLayouted = false;
	mIsReceiveOutsideInputEvent = false;

	mWindow = wnd;
	mLayoutParams = 0;
	mParent = 0;
	mICHolder = 0;

	mBackgroundDrawable = nullptr;
	mForegroundDrawable = nullptr;
	mClickListener = nullptr;
	mAnimator = nullptr;

	mClickPerformer = new ClickPerformer(this);
}


UWidget::~UWidget()
{
	delete mClickPerformer;

	if (mICHolder != nullptr)
		delete mICHolder;
}


UWidgetAnimator *UWidget::animate()
{
	if (mAnimator == nullptr)
		mAnimator = new UWidgetAnimator(this);

	return mAnimator;
}

void UWidget::setX(float x)
{
	setTranslateX(x - mLeft);
}

void UWidget::setY(float y)
{
	setTranslateY(y - mTop);
}

void UWidget::setAlpha(float alpha)
{
	mAlpha = alpha;
}

void UWidget::setScaleX(float sx)
{
	mScaleX = sx;
}

void UWidget::setScaleY(float sy)
{
	mScaleY = sy;
}

void UWidget::setTranslateX(float tx)
{
	mTranslateX = tx;
}

void UWidget::setTranslateY(float ty)
{
	mTranslateY = ty;
}

void UWidget::setPivotX(float px)
{
	mPivotX = px;
}

void UWidget::setPivotY(float py)
{
	mPivotY = py;
}

void UWidget::setRevealType(int type)
{
	mRevealType = type;
}

void UWidget::setHasReveal(bool reveal)
{
	mHasReveal = reveal;
}

void UWidget::setRevealRadius(float radius)
{
	mRevealRadius = radius;
}

void UWidget::setRevealCenterX(float cx)
{
	mRevealCenterX = cx;
}

void UWidget::setRevealCenterY(float cy)
{
	mRevealCenterY = cy;
}

void UWidget::setRevealWidthRadius(float widthRadius)
{
	mRevealWidthRadius = widthRadius;
}

void UWidget::setRevealHeightRadius(float heightRadius)
{
	mRevealHeightRadius = heightRadius;
}

void UWidget::setScrollX(int x)
{
	if (mScrollX != x)
	{
		mScrollX = x;
		this->invalidate();
	}
}

void UWidget::setScrollY(int y)
{
	if (mScrollY != y)
	{
		mScrollY = y;
		this->invalidate();
	}
}

void UWidget::setMeasuredDimension(int width, int height)
{
	mMeasuredWidth = width;
	mMeasuredHeight = height;
}

void UWidget::setVisibility(int visibility)
{
	if (visibility == mVisibility)
		return;

	mVisibility = visibility;

	if (mVisibility != VISIBLE)
	{
		discardFocus();
		discardPendingOperations();
	}

	this->requestLayout();
	this->invalidate();
	this->onVisibilityChanged(visibility);
}

void UWidget::setEnabled(bool enable)
{
	if (enable == mIsEnable)
		return;

	mIsEnable = enable;

	if (!mIsEnable)
	{
		discardFocus();
		discardPendingOperations();
	}

	this->invalidate();
}

void UWidget::setBackground(UDrawable *drawable)
{
	mBackgroundDrawable = drawable;

	this->invalidate();
}

void UWidget::setForeground(UDrawable *drawable)
{
	mForegroundDrawable = drawable;

	this->invalidate();
}

void UWidget::setPadding(int left, int top, int right, int bottom)
{
	if (mPaddingLeft == left 
		&& mPaddingTop == top 
		&& mPaddingRight == right 
		&& mPaddingBottom == bottom)
		return;

	mPaddingLeft = left;
	mPaddingTop = top;
	mPaddingRight = right;
	mPaddingBottom = bottom;

	this->requestLayout();
	this->invalidate();
}

void UWidget::setLayoutParams(ULayoutParams *params)
{
	if (params == nullptr)
		throw std::invalid_argument("setLayoutParams: null param");

	mLayoutParams = params;

	this->requestLayout();
	this->invalidate();
}

void UWidget::setIsInputEventAtLast(bool isInput)
{
	mIsInputEventAtLast = isInput;
}

void UWidget::setPressed(bool pressed)
{
	if (mIsPressed == pressed)
		return;

	mIsPressed = pressed;

	this->invalidate();
}

void UWidget::setCurrentCursor(LPCWSTR cursor)
{
	mWindow->setCurrentCursor(cursor);
}

void UWidget::setFocusable(bool focusable)
{
	if (mIsFocusable == focusable)
		return;

	mIsFocusable = focusable;

	if (!focusable)
	{
		this->discardFocus();
	}
}

void UWidget::setElevation(float elevation)
{
	if (elevation == mElevation)
		return;

	mElevation = elevation;

	this->requestLayout();
	this->invalidate();
}

void UWidget::setReceiveOutsideInputEvent(bool receive)
{
	mIsReceiveOutsideInputEvent = receive;
}

void UWidget::setParent(UWidget *parent)
{
	mParent = parent;
}

void UWidget::setMinimumWidth(int width)
{
	mMinimumWidth = width;
}

void UWidget::setMinimumHeight(int height)
{
	mMinimumHeight = height;
}

void UWidget::setOnClickListener(OnClickListener *l)
{
	mClickListener = l;
}


int UWidget::getId()
{
	return mId;
}

float UWidget::getX()
{
	return mLeft + getTranslateX();
}

float UWidget::getY()
{
	return mTop + getTranslateY();
}

float UWidget::getAlpha()
{
	return mAlpha;
}

float UWidget::getScaleX()
{
	return mScaleX;
}

float UWidget::getScaleY()
{
	return mScaleY;
}

float UWidget::getTranslateX()
{
	return mTranslateX;
}

float UWidget::getTranslateY()
{
	return mTranslateY;
}

float UWidget::getPivotX()
{
	return mPivotX;
}

float UWidget::getPivotY()
{
	return mPivotY;
}

int UWidget::getScrollX()
{
	return mScrollX;
}

int UWidget::getScrollY()
{
	return mScrollY;
}

int UWidget::getLeft()
{
	return mLeft;
}

int UWidget::getTop()
{
	return mTop;
}

int UWidget::getRight()
{
	return mRight;
}

int UWidget::getBottom()
{
	return mBottom;
}

int UWidget::getWidth()
{
	return mRight - mLeft;
}

int UWidget::getHeight()
{
	return mBottom - mTop;
}

int UWidget::getMeasuredWidth()
{
	return mMeasuredWidth;
}

int UWidget::getMeasuredHeight()
{
	return mMeasuredHeight;
}

int UWidget::getMinimumWidth()
{
	return mMinimumWidth;
}

int UWidget::getMinimumHeight()
{
	return mMinimumHeight;
}

float UWidget::getElevation()
{
	return mElevation;
}

int UWidget::getVisibility()
{
	return mVisibility;
}


int UWidget::getPaddingLeft()
{
	return mPaddingLeft;
}

int UWidget::getPaddingTop()
{
	return mPaddingTop;
}

int UWidget::getPaddingRight()
{
	return mPaddingRight;
}

int UWidget::getPaddingBottom()
{
	return mPaddingBottom;
}


ULayoutParams *UWidget::getLayoutParams()
{
	return mLayoutParams;
}

UWidget *UWidget::getParent()
{
	return mParent;
}


UWindow *UWidget::getWindow()
{
	return mWindow;
}


UDrawable *UWidget::getBackground()
{
	return mBackgroundDrawable;
}

UDrawable *UWidget::getForeground()
{
	return mForegroundDrawable;
}


URect UWidget::getBound()
{
	return URect(mLeft, mTop, mRight, mBottom);
}

URect UWidget::getBoundInWindow()
{
	URect bound = this->getBound();

	UWidget *parent = mParent;
	while (parent)
	{
		URect parentBound = parent->getBound();
		bound.left += parentBound.left;
		bound.top += parentBound.top;
		bound.right += parentBound.left;
		bound.bottom += parentBound.top;

		parent = parent->getParent();
	}

	return bound;
}

URect UWidget::getBoundInScreen()
{
	URect bound = this->getBoundInWindow();

	POINT pt;
	pt.x = bound.left;
	pt.y = bound.top;

	::ClientToScreen(mWindow->getWindowHandle(), &pt);

	int diffX = pt.x - bound.left;
	int diffY = pt.y - bound.top;

	bound.left += diffX;
	bound.top += diffY;
	bound.right += diffX;
	bound.bottom += diffY;

	return bound;
}


UWidget *UWidget::findWidgetById(int id)
{
	return nullptr;
}


bool UWidget::isEnabled()
{
	return mIsEnable;
}

bool UWidget::isAttachedToWindow()
{
	return mIsAttachdToWindow;
}

bool UWidget::isInputEventAtLast()
{
	return mIsInputEventAtLast;
}

bool UWidget::isPressed()
{
	return mIsPressed;
}

bool UWidget::hasFocus()
{
	return mHasFocus;
}

bool UWidget::isFocusable()
{
	return mIsFocusable;
}

bool UWidget::isLayouted()
{
	return mIsLayouted;
}

bool UWidget::isMouseInThis(UInputEvent *e)
{
	return (e->getMouseX() >= 0 && e->getMouseX() <= getWidth()
		&& e->getMouseY() >= 0 && e->getMouseY() <= getHeight());
}

bool UWidget::isGroupMouseInThis(UInputEvent *e)
{
	return (e->getMouseX() >= mLeft && e->getMouseX() <= mRight - 1
		&& e->getMouseY() >= mTop && e->getMouseY() <= mBottom - 1);
}

bool UWidget::isReceiveOutsideInputEvent()
{
	return mIsReceiveOutsideInputEvent;
}


void UWidget::scrollTo(int x, int y)
{
	if (mScrollX != x || mScrollY != y)
	{
		int oldScrollX = mScrollX;
		int oldScrollY = mScrollY;

		mScrollX = x;
		mScrollY = y;

		this->invalidate();
		this->onScrollChanged(x, y, oldScrollX, oldScrollY);
	}
}

void UWidget::scrollBy(int dx, int dy)
{
	this->scrollTo(mScrollX + dx, mScrollY + dy);
}

void UWidget::performClick()
{
	mClickListener->onClick(this);
}


void UWidget::draw(UCanvas *canvas)
{
	//应用动画变量。
	canvas->save();
	canvas->setOpacity(mAlpha*canvas->getOpacity());
	canvas->scale(mScaleX, mScaleY, mLeft + mPivotX, mTop + mPivotY);
	canvas->translate(mTranslateX, mTranslateY);

	//将背景绘制到bgBitmap上。
	UComPtr<ID2D1Bitmap> bgBitmap;
	if (mBackgroundDrawable != nullptr
		&& mBackgroundDrawable->getOpacity() != 0.f)
	{
		mWindow->getRenderer()->drawOnBitmap(
			mRight - mLeft, mBottom - mTop, &bgBitmap,
			[this, canvas](UComPtr<ID2D1RenderTarget> rt)
		{
			UCanvas bgCanvas(rt);
			bgCanvas.setOpacity(canvas->getOpacity());
			this->drawBackground(&bgCanvas);
		});
	}

	bool hasBg = (bgBitmap != nullptr);
	bool hasShadow = (hasBg && (mElevation > 0.f));

	//若有，使用layer应用reveal动画。
	//若某一widget正在进行reveal动画，则其child无法应用reveal动画。
	if (mHasReveal)
	{
		//圆形reveal动画。
		if (mRevealType == UWidgetAnimator::REVEAL_CIRCULE)
		{
			UComPtr<ID2D1EllipseGeometry> circleGeo;
			UDeviceManager::sD2DFactory->CreateEllipseGeometry(
				D2D1::Ellipse(
					D2D1::Point2F(mRevealCenterX, mRevealCenterY),
					mRevealRadius, mRevealRadius), &circleGeo);

			//在pushLayer之前绘制阴影。
			if (hasShadow)
			{
				ID2D1Geometry *geo = circleGeo.cast<ID2D1Geometry>().get();
				ID2D1BitmapBrush *brush = UDeviceManager::sBitmapBrush.get();
				brush->SetBitmap(bgBitmap.get());

				UComPtr<ID2D1Bitmap> bgRevealedBitmap;
				mWindow->getRenderer()->drawOnBitmap(
					mRight - mLeft, mBottom - mTop, &bgRevealedBitmap,
					[this, geo, brush](UComPtr<ID2D1RenderTarget> rt)
				{
					rt->FillGeometry(geo, brush);
				});

				mWindow->getRenderer()->drawShadow(mElevation, canvas->getOpacity(), bgRevealedBitmap.get());
				canvas->drawBitmap(bgRevealedBitmap.get());
			}

			canvas->pushLayer(circleGeo.get());

			if (hasBg && !hasShadow)
				canvas->drawBitmap(bgBitmap.get());
		}
		//矩形reveal动画。
		else if (mRevealType == UWidgetAnimator::REVEAL_RECT)
		{
			UComPtr<ID2D1RectangleGeometry> rectGeo;
			UDeviceManager::sD2DFactory->CreateRectangleGeometry(
				D2D1::RectF(
					mRevealCenterX - mRevealWidthRadius,
					mRevealCenterY - mRevealHeightRadius,
					mRevealCenterX + mRevealWidthRadius,
					mRevealCenterY + mRevealHeightRadius), &rectGeo);

			//在pushLayer之前绘制阴影。
			if (hasShadow)
			{
				ID2D1Geometry *geo = rectGeo.cast<ID2D1Geometry>().get();
				ID2D1BitmapBrush *brush = UDeviceManager::sBitmapBrush.get();
				brush->SetBitmap(bgBitmap.get());

				UComPtr<ID2D1Bitmap> bgRevealedBitmap;
				mWindow->getRenderer()->drawOnBitmap(
					mRight - mLeft, mBottom - mTop, &bgRevealedBitmap,
					[this, geo, brush](UComPtr<ID2D1RenderTarget> rt)
				{
					rt->FillGeometry(geo, brush);
				});

				mWindow->getRenderer()->drawShadow(mElevation, canvas->getOpacity(), bgRevealedBitmap.get());
				canvas->drawBitmap(bgRevealedBitmap.get());
			}

			canvas->pushLayer(rectGeo.get());

			if(hasBg && !hasShadow)
				canvas->drawBitmap(bgBitmap.get());
		}
	}
	//没有reveal动画，直接绘制背景和阴影。
	else
	{
		if (hasBg)
		{
			if (hasShadow)
				mWindow->getRenderer()->drawShadow(
					mElevation, canvas->getOpacity(), bgBitmap.get());
			canvas->drawBitmap(bgBitmap.get());
		}
	}

	//应用Padding。
	canvas->save();
	canvas->translate(mPaddingLeft, mPaddingTop);
	
	//裁剪。
	canvas->pushClip(D2D1::RectF(0, 0,
		mMeasuredWidth - mPaddingLeft - mPaddingRight,
		mMeasuredHeight - mPaddingTop - mPaddingBottom));
	canvas->translate(-mScrollX, -mScrollY);

	//绘制自身。
	this->onDraw(canvas);
	//绘制孩子。
	this->dispatchDraw(canvas);

	canvas->popClip();
	canvas->restore();

	//绘制前景。
	this->drawForeground(canvas);

	if (mHasReveal)
		canvas->popLayer();

	canvas->restore();
}

void UWidget::drawBackgroundWithShadow(UCanvas *canvas)
{
	if (mBackgroundDrawable != nullptr
		&& mBackgroundDrawable->getOpacity() != 0.f)
	{
		if (mElevation == 0.f)
			drawBackground(canvas);
		else
		{
			mBackgroundDrawable->setBound(
				0, 0, mRight - mLeft, mBottom - mTop);
			D2D1_RECT_F bkRect = mBackgroundDrawable->getBound();

			mWindow->getRenderer()->drawWithShadow(
				mElevation, bkRect.right - bkRect.left, bkRect.bottom - bkRect.top,
				[this, canvas](UComPtr<ID2D1RenderTarget> rt)
			{
				UCanvas bkCanvas(rt);
				bkCanvas.setOpacity(canvas->getOpacity());
				this->drawBackground(&bkCanvas);
			});
		}
	}
}

void UWidget::drawBackground(UCanvas *canvas)
{
	if (mBackgroundDrawable != nullptr
		&& mBackgroundDrawable->getOpacity() != 0.f)
	{
		mBackgroundDrawable->setBound(0, 0, mRight - mLeft, mBottom - mTop);
		mBackgroundDrawable->draw(canvas);
	}
}

void UWidget::drawForeground(UCanvas *canvas)
{
	if (mForegroundDrawable != nullptr
		&& mForegroundDrawable->getOpacity() != 0.f)
	{
		mForegroundDrawable->setBound(0.f, 0.f, mRight - mLeft, mBottom - mTop);
		mForegroundDrawable->draw(canvas);
	}
}


void UWidget::measure(int width, int height, int widthMode, int heightMode)
{
	this->onMeasure(width, height, widthMode, heightMode);
}


void UWidget::layout(int left, int top, int right, int bottom)
{
	bool sizeChanged = false;

	int width = right - left;
	int height = bottom - top;
	int oldWidth = mRight - mLeft;
	int oldHeight = mBottom - mTop;

	bool changed =
		left != mLeft
		|| top != mTop
		|| right != mRight
		|| bottom != mBottom;
	if (changed)
	{
		sizeChanged =
			oldWidth != width
			|| oldHeight != height;

		mLeft = left;
		mTop = top;
		mRight = right;
		mBottom = bottom;

		if (sizeChanged)
			this->onSizeChanged(width, height, oldWidth, oldHeight);
	}

	this->onLayout(
		changed, sizeChanged, 
		left, top, right, bottom);

	mIsLayouted = true;
}


void UWidget::invalidate()
{
	this->invalidate(mLeft, mTop, mRight, mBottom);
}

void UWidget::invalidate(URect rect)
{
	this->invalidate(rect.left, rect.top, rect.right, rect.bottom);
}

void UWidget::invalidate(int left, int top, int right, int bottom)
{
	//TODO:应只刷新脏区域。
	//this->getWindow()->performRefresh(left, top, right, bottom);

	//直接绘制。(实时)
	//this->getWindow()->performRefresh();

	//加入消息队列，等待下一帧。(非实时)
	this->getWindow()->invalidate();
}

void UWidget::requestLayout()
{
	//直接布局。(实时)
	//this->getWindow()->performLayout();

	//加入消息队列，等待下一帧。(非实时)
	this->getWindow()->requestLayout();
}

void UWidget::requestFocus()
{
	if (!mIsFocusable || mHasFocus)
		return;

	//先取消其他widget的焦点。
	UWidget *prevHolder = mWindow->getKeyboardHolder();
	if (prevHolder != 0)
		prevHolder->discardFocus();

	//获取焦点。
	mWindow->captureKeyboard(this);

	mHasFocus = true;
	this->onFocusChanged(true);
}

void UWidget::discardFocus()
{
	//舍弃焦点(如果有的话)。
	if (mHasFocus)
	{
		mHasFocus = false;
		mWindow->releaseKeyboard();
		this->onFocusChanged(false);
	}

	this->dispatchDiscardFocus();
}

void UWidget::discardPendingOperations()
{
	mWindow->getCycler()->removeCallbacks(mClickPerformer);

	this->dispatchDiscardPendingOperations();
}


void UWidget::dispatchDraw(UCanvas *canvas)
{
}

void UWidget::dispatchDiscardFocus()
{
}

void UWidget::dispatchDiscardPendingOperations()
{
}


bool UWidget::dispatchInputEvent(UInputEvent *e)
{
	e->setMouseX(e->getMouseX() - mLeft);
	e->setMouseY(e->getMouseY() - mTop);

	return this->onInputEvent(e);
}

void UWidget::dispatchWindowFocusChanged(bool windowFocus)
{
	this->onWindowFocusChanged(windowFocus);
}


void UWidget::onAttachedToWindow()
{
	mIsAttachdToWindow = true;

	if (mICHolder != nullptr)
	{
		mICHolder->pushEditor();
	}
}

void UWidget::onDetachedFromWindow()
{
	mIsAttachdToWindow = false;

	if (mICHolder != nullptr)
	{
		mICHolder->popEditor();
	}

	if (mAnimator != nullptr)
		mAnimator->cancel();
}


void UWidget::onDraw(UCanvas *canvas)
{
}

bool UWidget::onInputEvent(UInputEvent *e)
{
	bool shouldRefresh = false;

	switch (e->getEvent())
	{
	case UInputEvent::EVENT_MOUSE_KEY_DOWN:
		mWindow->captureMouse(this);
		if (mIsFocusable)
			this->requestFocus();
		if (e->getMouseKey() == UInputEvent::KEY_MOUSE_LEFT)
		{
			this->setPressed(true);
			if (mForegroundDrawable)
			{
				mForegroundDrawable->setHotspot(e->getMouseX(), e->getMouseY());
				shouldRefresh=mForegroundDrawable->setState(UDrawable::STATE_PRESSED);
			}
			if (mBackgroundDrawable)
			{
				mBackgroundDrawable->setHotspot(e->getMouseX(), e->getMouseY());
				shouldRefresh=mBackgroundDrawable->setState(UDrawable::STATE_PRESSED);
			}
		}
		if (shouldRefresh)
			invalidate();
		return true;

	case UInputEvent::EVENT_MOUSE_MOVE:
		if (!isPressed())
		{
			if (mForegroundDrawable)
			{
				//mForegroundDrawable->setHotspot(e->getMouseX(), e->getMouseY());
				shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_HOVERED);
			}
			if (mBackgroundDrawable)
			{
				//mBackgroundDrawable->setHotspot(e->getMouseX(), e->getMouseY());
				shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_HOVERED);
			}
		}
		if (shouldRefresh)
			invalidate();
		return true;

	case UInputEvent::EVENT_MOUSE_SCROLL_ENTER:
		if (mForegroundDrawable)
			shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_HOVERED);
		if (mBackgroundDrawable)
			shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_HOVERED);
		if (shouldRefresh)
			invalidate();
		return true;

	case UInputEvent::EVENT_MOUSE_LEAVE:
		if (mForegroundDrawable)
			shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_NONE);
		if (mBackgroundDrawable)
			shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_NONE);
		if (shouldRefresh)
			invalidate();
		return true;

	case UInputEvent::EVENT_MOUSE_KEY_UP:
		mWindow->releaseMouse();
		if (e->getMouseKey() == UInputEvent::KEY_MOUSE_LEFT)
		{
			bool pressed = false;
			if (isPressed())
			{
				pressed = true;
				this->setPressed(false);
			}

			if (isMouseInThis(e))
			{
				if (mForegroundDrawable)
				{
					mForegroundDrawable->setHotspot(e->getMouseX(), e->getMouseY());
					shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_HOVERED);
				}
				if (mBackgroundDrawable)
				{
					mBackgroundDrawable->setHotspot(e->getMouseX(), e->getMouseY());
					shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_HOVERED);
				}

				if (pressed && mClickListener)
				{
					mWindow->getCycler()->removeCallbacks(mClickPerformer);
					mWindow->getCycler()->post(mClickPerformer);
				}
			}
			else
			{
				if (mForegroundDrawable)
					shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_NONE);
				if (mBackgroundDrawable)
					shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_NONE);
			}
		}
		if (shouldRefresh)
			invalidate();
		return true;

	case UInputEvent::EVENT_CANCEL:
		mWindow->releaseMouse();
		this->setPressed(false);
		if (mForegroundDrawable)
			shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_NONE);
		if (mBackgroundDrawable)
			shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_NONE);
		if (shouldRefresh)
			invalidate();
		return true;
	}

	return false;
}

void UWidget::onMeasure(int width, int height, int widthMode, int heightMode)
{
	setMeasuredDimension(width > mMinimumWidth ? width : mMinimumWidth,
		height > mMinimumHeight ? height : mMinimumHeight);
}

void UWidget::onLayout(
	bool changed, bool sizeChanged, 
	int left, int top, int right, int bottom)
{
}


bool UWidget::onCheckIsTextEditor()
{
	return false;
}

UInputConnection *UWidget::onCreateInputConnection()
{
	return 0;
}


void UWidget::onSizeChanged(int width, int height, int oldWidth, int oldHeight)
{
}

void UWidget::onVisibilityChanged(int visibility)
{
}

void UWidget::onFocusChanged(bool getFocus)
{
	if (getFocus)
	{
		if (this->onCheckIsTextEditor())
		{
			if (mICHolder == nullptr)
				mICHolder = this->onCreateInputConnection();

			if (mICHolder != nullptr)
			{
				UTsfManager *tsfMgr = this->mWindow->getApplication()->getTsfManager();
				mICHolder->initialization(tsfMgr);
				mICHolder->pushEditor();
				mICHolder->mount(tsfMgr);
			}
		}
	}
	else
	{
		if (mICHolder != nullptr)
		{
			UTsfManager *tsfMgr = this->mWindow->getApplication()->getTsfManager();
			mICHolder->unmount(tsfMgr);
			mICHolder->terminateComposition();
		}
	}

	bool shouldRefresh = false;
	if (mBackgroundDrawable)
		shouldRefresh = mBackgroundDrawable->setParentFocus(getFocus);
	if (mForegroundDrawable)
		shouldRefresh = mForegroundDrawable->setParentFocus(getFocus);

	if (shouldRefresh)
		invalidate();
}

void UWidget::onWindowFocusChanged(bool windowFocus)
{
	if (!hasFocus())
		return;

	if (windowFocus)
	{
		if (this->onCheckIsTextEditor())
		{
			if (mICHolder == nullptr)
				mICHolder = this->onCreateInputConnection();

			if (mICHolder != nullptr)
			{
				UTsfManager *tsfMgr = this->mWindow->getApplication()->getTsfManager();
				mICHolder->initialization(tsfMgr);
				mICHolder->mount(tsfMgr);
			}
		}
	}
	else
	{
		if (mICHolder != nullptr)
		{
			UTsfManager *tsfMgr = this->mWindow->getApplication()->getTsfManager();
			mICHolder->unmount(tsfMgr);
			mICHolder->terminateComposition();
		}
	}
}

void UWidget::onScrollChanged(int scrollX, int scrollY, int oldScrollX, int oldScrollY)
{
}