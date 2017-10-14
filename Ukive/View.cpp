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
#include "LayoutParams.h"
#include "OnClickListener.h"
#include "ViewAnimator.h"
#include "UInnerWindow.h"
#include "UDeviceManager.h"
#include "View.h"


View::View(UWindow *wnd)
	:View(wnd, UApplication::getWidgetUID())
{
}

View::View(UWindow *wnd, int id)
{
	if (wnd == nullptr)
		throw std::invalid_argument(
			"UWidget-Constructor(): null UWindow* param");

	mAlpha = 1;
	mScaleX = 1;
	mScaleY = 1;
	mTranslateX = 0;
	mTranslateY = 0;
	mPivotX = mPivotY = 0;

	mRevealType = ViewAnimator::REVEAL_CIRCULE;
	mHasReveal = false;
	mRevealRadius = 0;
	mRevealCenterX = mRevealCenterY = 0;
	mRevealWidthRadius = mRevealHeightRadius = 0;

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
	mCanConsumeMouseEvent = true;

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


View::~View()
{
	delete mClickPerformer;

	if (mICHolder != nullptr)
		delete mICHolder;
}


ViewAnimator *View::animate()
{
	if (mAnimator == nullptr)
		mAnimator = new ViewAnimator(this);

	return mAnimator;
}

void View::setX(double x)
{
	setTranslateX(x - mLeft);
}

void View::setY(double y)
{
	setTranslateY(y - mTop);
}

void View::setAlpha(double alpha)
{
	mAlpha = alpha;
}

void View::setScaleX(double sx)
{
	mScaleX = sx;
}

void View::setScaleY(double sy)
{
	mScaleY = sy;
}

void View::setTranslateX(double tx)
{
	mTranslateX = tx;
}

void View::setTranslateY(double ty)
{
	mTranslateY = ty;
}

void View::setPivotX(double px)
{
	mPivotX = px;
}

void View::setPivotY(double py)
{
	mPivotY = py;
}

void View::setRevealType(int type)
{
	mRevealType = type;
}

void View::setHasReveal(bool reveal)
{
	mHasReveal = reveal;
}

void View::setRevealRadius(double radius)
{
	mRevealRadius = radius;
}

void View::setRevealCenterX(double cx)
{
	mRevealCenterX = cx;
}

void View::setRevealCenterY(double cy)
{
	mRevealCenterY = cy;
}

void View::setRevealWidthRadius(double widthRadius)
{
	mRevealWidthRadius = widthRadius;
}

void View::setRevealHeightRadius(double heightRadius)
{
	mRevealHeightRadius = heightRadius;
}

void View::setScrollX(int x)
{
	if (mScrollX != x)
	{
		mScrollX = x;
		this->invalidate();
	}
}

void View::setScrollY(int y)
{
	if (mScrollY != y)
	{
		mScrollY = y;
		this->invalidate();
	}
}

void View::setMeasuredDimension(int width, int height)
{
	mMeasuredWidth = width;
	mMeasuredHeight = height;
}

void View::setVisibility(int visibility)
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

void View::setEnabled(bool enable)
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

void View::setBackground(UDrawable *drawable)
{
	mBackgroundDrawable = drawable;

	this->invalidate();
}

void View::setForeground(UDrawable *drawable)
{
	mForegroundDrawable = drawable;

	this->invalidate();
}

void View::setPadding(int left, int top, int right, int bottom)
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

void View::setLayoutParams(LayoutParams *params)
{
	if (params == nullptr)
		throw std::invalid_argument("setLayoutParams: null param");

	mLayoutParams = params;

	this->requestLayout();
	this->invalidate();
}

void View::setIsInputEventAtLast(bool isInput)
{
	mIsInputEventAtLast = isInput;
}

void View::setPressed(bool pressed)
{
	if (mIsPressed == pressed)
		return;

	mIsPressed = pressed;

	this->invalidate();
}

void View::setCurrentCursor(LPCWSTR cursor)
{
	mWindow->setCurrentCursor(cursor);
}

void View::setFocusable(bool focusable)
{
	if (mIsFocusable == focusable)
		return;

	mIsFocusable = focusable;

	if (!focusable)
	{
		this->discardFocus();
	}
}

void View::setElevation(float elevation)
{
	if (elevation == mElevation)
		return;

	mElevation = elevation;

	this->requestLayout();
	this->invalidate();
}

void View::setReceiveOutsideInputEvent(bool receive)
{
	mIsReceiveOutsideInputEvent = receive;
}

void View::setCanConsumeMouseEvent(bool enable)
{
	mCanConsumeMouseEvent = enable;

	if(!enable)
		mWindow->releaseMouse();
}

void View::setParent(View *parent)
{
	mParent = parent;
}

void View::setMinimumWidth(int width)
{
	mMinimumWidth = width;
}

void View::setMinimumHeight(int height)
{
	mMinimumHeight = height;
}

void View::setOnClickListener(OnClickListener *l)
{
	mClickListener = l;
}


int View::getId()
{
	return mId;
}

double View::getX()
{
	return mLeft + getTranslateX();
}

double View::getY()
{
	return mTop + getTranslateY();
}

double View::getAlpha()
{
	return mAlpha;
}

double View::getScaleX()
{
	return mScaleX;
}

double View::getScaleY()
{
	return mScaleY;
}

double View::getTranslateX()
{
	return mTranslateX;
}

double View::getTranslateY()
{
	return mTranslateY;
}

double View::getPivotX()
{
	return mPivotX;
}

double View::getPivotY()
{
	return mPivotY;
}

int View::getScrollX()
{
	return mScrollX;
}

int View::getScrollY()
{
	return mScrollY;
}

int View::getLeft()
{
	return mLeft;
}

int View::getTop()
{
	return mTop;
}

int View::getRight()
{
	return mRight;
}

int View::getBottom()
{
	return mBottom;
}

int View::getWidth()
{
	return mRight - mLeft;
}

int View::getHeight()
{
	return mBottom - mTop;
}

int View::getMeasuredWidth()
{
	return mMeasuredWidth;
}

int View::getMeasuredHeight()
{
	return mMeasuredHeight;
}

int View::getMinimumWidth()
{
	return mMinimumWidth;
}

int View::getMinimumHeight()
{
	return mMinimumHeight;
}

float View::getElevation()
{
	return mElevation;
}

int View::getVisibility()
{
	return mVisibility;
}


int View::getPaddingLeft()
{
	return mPaddingLeft;
}

int View::getPaddingTop()
{
	return mPaddingTop;
}

int View::getPaddingRight()
{
	return mPaddingRight;
}

int View::getPaddingBottom()
{
	return mPaddingBottom;
}


LayoutParams *View::getLayoutParams()
{
	return mLayoutParams;
}

View *View::getParent()
{
	return mParent;
}


UWindow *View::getWindow()
{
	return mWindow;
}


UDrawable *View::getBackground()
{
	return mBackgroundDrawable;
}

UDrawable *View::getForeground()
{
	return mForegroundDrawable;
}


URect View::getBound()
{
	return URect(mLeft, mTop, mRight, mBottom);
}

URect View::getBoundInWindow()
{
	URect bound = this->getBound();

	View *parent = mParent;
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

URect View::getBoundInScreen()
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


View *View::findWidgetById(int id)
{
	return nullptr;
}


bool View::isEnabled()
{
	return mIsEnable;
}

bool View::isAttachedToWindow()
{
	return mIsAttachdToWindow;
}

bool View::isInputEventAtLast()
{
	return mIsInputEventAtLast;
}

bool View::isPressed()
{
	return mIsPressed;
}

bool View::hasFocus()
{
	return mHasFocus;
}

bool View::isFocusable()
{
	return mIsFocusable;
}

bool View::isLayouted()
{
	return mIsLayouted;
}

bool View::isMouseInThis(UInputEvent *e)
{
	return (e->getMouseX() >= 0 && e->getMouseX() <= getWidth()
		&& e->getMouseY() >= 0 && e->getMouseY() <= getHeight());
}

bool View::isGroupMouseInThis(UInputEvent *e)
{
	return (e->getMouseX() >= mLeft && e->getMouseX() <= mRight - 1
		&& e->getMouseY() >= mTop && e->getMouseY() <= mBottom - 1);
}

bool View::isReceiveOutsideInputEvent()
{
	return mIsReceiveOutsideInputEvent;
}

bool View::canConsumeMouseEvent()
{
	return mCanConsumeMouseEvent;
}


void View::scrollTo(int x, int y)
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

void View::scrollBy(int dx, int dy)
{
	this->scrollTo(mScrollX + dx, mScrollY + dy);
}

void View::performClick()
{
	mClickListener->onClick(this);
}


void View::draw(UCanvas *canvas)
{
	//应用动画变量。
	canvas->save();
	canvas->setOpacity(mAlpha*canvas->getOpacity());
	canvas->scale(mScaleX, mScaleY, mLeft + mPivotX, mTop + mPivotY);
	canvas->translate(
		static_cast<float>(mTranslateX), 
		static_cast<float>(mTranslateY));

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
		if (mRevealType == ViewAnimator::REVEAL_CIRCULE)
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
		else if (mRevealType == ViewAnimator::REVEAL_RECT)
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

void View::drawBackgroundWithShadow(UCanvas *canvas)
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

void View::drawBackground(UCanvas *canvas)
{
	if (mBackgroundDrawable != nullptr
		&& mBackgroundDrawable->getOpacity() != 0.f)
	{
		mBackgroundDrawable->setBound(0, 0, mRight - mLeft, mBottom - mTop);
		mBackgroundDrawable->draw(canvas);
	}
}

void View::drawForeground(UCanvas *canvas)
{
	if (mForegroundDrawable != nullptr
		&& mForegroundDrawable->getOpacity() != 0.f)
	{
		mForegroundDrawable->setBound(0.f, 0.f, mRight - mLeft, mBottom - mTop);
		mForegroundDrawable->draw(canvas);
	}
}


void View::measure(int width, int height, int widthMode, int heightMode)
{
	this->onMeasure(width, height, widthMode, heightMode);
}


void View::layout(int left, int top, int right, int bottom)
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


void View::invalidate()
{
	this->invalidate(mLeft, mTop, mRight, mBottom);
}

void View::invalidate(URect rect)
{
	this->invalidate(rect.left, rect.top, rect.right, rect.bottom);
}

void View::invalidate(int left, int top, int right, int bottom)
{
	//TODO:应只刷新脏区域。
	//this->getWindow()->performRefresh(left, top, right, bottom);

	//直接绘制。(实时)
	//this->getWindow()->performRefresh();

	//加入消息队列，等待下一帧。(非实时)
	this->getWindow()->invalidate();
}

void View::requestLayout()
{
	//直接布局。(实时)
	//this->getWindow()->performLayout();

	//加入消息队列，等待下一帧。(非实时)
	this->getWindow()->requestLayout();
}

void View::requestFocus()
{
	if (!mIsFocusable || mHasFocus)
		return;

	//先取消其他widget的焦点。
	View *prevHolder = mWindow->getKeyboardHolder();
	if (prevHolder != 0)
		prevHolder->discardFocus();

	//获取焦点。
	mWindow->captureKeyboard(this);

	mHasFocus = true;
	this->onFocusChanged(true);
}

void View::discardFocus()
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

void View::discardPendingOperations()
{
	mWindow->getCycler()->removeCallbacks(mClickPerformer);

	this->dispatchDiscardPendingOperations();
}


void View::dispatchDraw(UCanvas *canvas)
{
}

void View::dispatchDiscardFocus()
{
}

void View::dispatchDiscardPendingOperations()
{
}


bool View::dispatchInputEvent(UInputEvent *e)
{
	e->setMouseX(e->getMouseX() - mLeft);
	e->setMouseY(e->getMouseY() - mTop);

	return this->onInputEvent(e);
}

void View::dispatchWindowFocusChanged(bool windowFocus)
{
	this->onWindowFocusChanged(windowFocus);
}


void View::onAttachedToWindow()
{
	mIsAttachdToWindow = true;

	if (mICHolder != nullptr)
	{
		mICHolder->pushEditor();
	}
}

void View::onDetachedFromWindow()
{
	mIsAttachdToWindow = false;

	if (mICHolder != nullptr)
	{
		mICHolder->popEditor();
	}

	if (mAnimator != nullptr)
		mAnimator->cancel();
}


void View::onDraw(UCanvas *canvas)
{
}

bool View::onInputEvent(UInputEvent *e)
{
	bool shouldRefresh = false;

	switch (e->getEvent())
	{
	case UInputEvent::EVENT_MOUSE_KEY_DOWN:
		if (mCanConsumeMouseEvent)
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
		return mCanConsumeMouseEvent;

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
		return mCanConsumeMouseEvent;

	case UInputEvent::EVENT_MOUSE_SCROLL_ENTER:
		if (mForegroundDrawable)
			shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_HOVERED);
		if (mBackgroundDrawable)
			shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_HOVERED);
		if (shouldRefresh)
			invalidate();
		return mCanConsumeMouseEvent;

	case UInputEvent::EVENT_MOUSE_LEAVE:
		if (mForegroundDrawable)
			shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_NONE);
		if (mBackgroundDrawable)
			shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_NONE);
		if (shouldRefresh)
			invalidate();
		return mCanConsumeMouseEvent;

	case UInputEvent::EVENT_MOUSE_KEY_UP:
		if (mCanConsumeMouseEvent)
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
		return mCanConsumeMouseEvent;

	case UInputEvent::EVENT_CANCEL:
		if (mCanConsumeMouseEvent)
			mWindow->releaseMouse();
		this->setPressed(false);
		if (mForegroundDrawable)
			shouldRefresh = mForegroundDrawable->setState(UDrawable::STATE_NONE);
		if (mBackgroundDrawable)
			shouldRefresh = mBackgroundDrawable->setState(UDrawable::STATE_NONE);
		if (shouldRefresh)
			invalidate();
		return mCanConsumeMouseEvent;
	}

	return false;
}

void View::onMeasure(int width, int height, int widthMode, int heightMode)
{
	setMeasuredDimension(width > mMinimumWidth ? width : mMinimumWidth,
		height > mMinimumHeight ? height : mMinimumHeight);
}

void View::onLayout(
	bool changed, bool sizeChanged, 
	int left, int top, int right, int bottom)
{
}


bool View::onCheckIsTextEditor()
{
	return false;
}

UInputConnection *View::onCreateInputConnection()
{
	return 0;
}


void View::onSizeChanged(int width, int height, int oldWidth, int oldHeight)
{
}

void View::onVisibilityChanged(int visibility)
{
}

void View::onFocusChanged(bool getFocus)
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

void View::onWindowFocusChanged(bool windowFocus)
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

void View::onScrollChanged(int scrollX, int scrollY, int oldScrollX, int oldScrollY)
{
}