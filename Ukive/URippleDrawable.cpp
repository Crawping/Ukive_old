#include "UCommon.h"
#include "UWindow.h"
#include "UCanvas.h"
#include "UColor.h"
#include "URenderer.h"
#include "UTransition.h"
#include "URippleDrawable.h"


URippleDrawable::URippleDrawable(UWindow *wnd)
	:ULayerDrawable(wnd)
{
	mAlpha = 0;
	mTintColor = D2D1::ColorF(0, 0);

	mDownAnimator = new UAnimator(wnd->getAnimationManager());
	mUpAnimator = new UAnimator(wnd->getAnimationManager());

	mHoverAnimator = new UAnimator(wnd->getAnimationManager());
	mLeaveAnimator = new UAnimator(wnd->getAnimationManager());

	mRippleAnimator=new UAnimator(wnd->getAnimationManager());
}

URippleDrawable::~URippleDrawable()
{
	mRippleAnimator->stop();
	delete mRippleAnimator;

	mHoverAnimator->setOnValueChangedListener(0, nullptr);
	mHoverAnimator->stop();
	delete mHoverAnimator;

	mLeaveAnimator->setOnValueChangedListener(0, nullptr);
	mLeaveAnimator->stop();
	delete mLeaveAnimator;

	mUpAnimator->setOnValueChangedListener(0, nullptr);
	mUpAnimator->stop();
	delete mUpAnimator;

	mDownAnimator->setOnValueChangedListener(0, nullptr);
	mDownAnimator->stop();
	delete mDownAnimator;
}


void URippleDrawable::setTintColor(D2D1_COLOR_F tint)
{
	mTintColor = tint;
}


void URippleDrawable::draw(UCanvas *canvas)
{
	D2D1_RECT_F bound = getBound();
	D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0, mAlpha);

	//绘制底色、alpha和ripple。
	UComPtr<ID2D1Bitmap> contentBitmap;
	mOwnerWindow->getRenderer()->drawOnBitmap(
		bound.right - bound.left, bound.bottom - bound.top, &contentBitmap,
		[this, bound, color, canvas](UComPtr<ID2D1RenderTarget> rt)
	{
		UCanvas offCanvas = UCanvas(rt);
		offCanvas.setOpacity(canvas->getOpacity());

		D2D1_RECT_F _bound = bound;
		D2D1_COLOR_F _color = color;

		offCanvas.fillRect(_bound, mTintColor);
		offCanvas.fillRect(_bound, _color);

		if (getState() == STATE_HOVERED
			&& getPrevState() == STATE_PRESSED)
		{
			D2D1_COLOR_F rippleColor = D2D1::ColorF(0,
				mRippleAnimator->getValue(1));

			offCanvas.fillCircle(
				mStartX, mStartY,
				mRippleAnimator->getValue(0), rippleColor);
		}
	});

	if (mDrawableList.empty())
	{
		canvas->drawBitmap(contentBitmap.get());
	}
	else
	{
		//绘制mask，以该mask确定背景形状以及ripple的扩散边界。
		UComPtr<ID2D1Bitmap> maskBitmap;
		mOwnerWindow->getRenderer()->drawOnBitmap(
			bound.right - bound.left, bound.bottom - bound.top, &maskBitmap,
			[this, bound, canvas](UComPtr<ID2D1RenderTarget> rt)
		{
			UCanvas offCanvas = UCanvas(rt);
			offCanvas.setOpacity(canvas->getOpacity());

			ULayerDrawable::draw(&offCanvas);
		});

		canvas->drawBitmap(maskBitmap.get());

		canvas->fillOpacityMask(
			bound.right - bound.left, bound.bottom - bound.top,
			maskBitmap.get(), contentBitmap.get());
	}
}

bool URippleDrawable::onStateChanged(int newState, int prevState)
{
	switch (newState)
	{
	case STATE_NONE:
	{
		::OutputDebugStringW(L"STATE_NONE\n");
		if (prevState == STATE_HOVERED)
		{
			mHoverAnimator->stop();
			mAlpha = mHoverAnimator->getValue(0);

			mLeaveAnimator->addVariable(0, mAlpha, 0, 1);
			mLeaveAnimator->setOnValueChangedListener(0, dynamic_cast<OnValueChangedListener*>(this));
			mLeaveAnimator->startTransition(0, UTransition::linearTransition(HOVER_LEAVE_SEC, 0));
		}
		else if (prevState == STATE_PRESSED)
		{
			mDownAnimator->stop();
			mAlpha = mDownAnimator->getValue(0);

			mUpAnimator->addVariable(0, mAlpha, 0, 1);
			mUpAnimator->setOnValueChangedListener(0, static_cast<OnValueChangedListener*>(this));
			mUpAnimator->startTransition(0, UTransition::linearTransition(DOWN_UP_SEC, 0));
		}
		break;
	}

	case STATE_HOVERED:
		::OutputDebugStringW(L"STATE_HOVERED\n");
		if (prevState == STATE_NONE)
		{
			mAlpha = 0;
			mHoverAnimator->addVariable(0, 0, 0, 1);
			mHoverAnimator->setOnValueChangedListener(0, static_cast<OnValueChangedListener*>(this));
			mHoverAnimator->startTransition(0, UTransition::linearTransition(HOVER_LEAVE_SEC, HOVER_ALPHA));
		}
		else if (prevState == STATE_PRESSED)
		{
			mDownAnimator->stop();
			mAlpha = mDownAnimator->getValue(0);

			mUpAnimator->addVariable(0, mAlpha, 0, 1);
			mUpAnimator->setOnValueChangedListener(0, static_cast<OnValueChangedListener*>(this));
			mUpAnimator->startTransition(0, UTransition::linearTransition(DOWN_UP_SEC, HOVER_ALPHA));

			mRippleAnimator->reset();
			mRippleAnimator->addVariable(0, 0, 0, 100);
			mRippleAnimator->addVariable(1, 0.1, 0, 1);
			mRippleAnimator->addTransition(0, UTransition::linearTransition(0.5, 100));
			mRippleAnimator->addTransition(1, UTransition::linearTransition(0.5, 0));
			mRippleAnimator->start();
		}
		break;

	case STATE_PRESSED:
		::OutputDebugStringW(L"STATE_PRESSED\n");
		mHoverAnimator->stop();
		mAlpha = mHoverAnimator->getValue(0);

		mDownAnimator->addVariable(0, mAlpha, 0, 1);
		mDownAnimator->setOnValueChangedListener(0, static_cast<OnValueChangedListener*>(this));
		mDownAnimator->startTransition(0, UTransition::linearTransition(DOWN_UP_SEC, DOWN_ALPHA));
		break;

	default:
		break;
	}

	return false;
}

float URippleDrawable::getOpacity()
{
	return 1.0f;
}


void URippleDrawable::onValueChanged(
	unsigned int varIndex,
	IUIAnimationStoryboard *storyboard,
	IUIAnimationVariable *variable,
	double newValue, double previousValue)
{
	mAlpha = newValue;
}

void URippleDrawable::onIntegerValueChanged(
	unsigned int varIndex,
	IUIAnimationStoryboard *storyboard,
	IUIAnimationVariable *variable,
	int newValue, int previousValue)
{

}