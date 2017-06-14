#include "UCommon.h"
#include "UMath.h"
#include "UWidget.h"
#include "UCycler.h"
#include "UWindow.h"
#include "UCanvas.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "UTextBlink.h"


UTextBlink::UTextBlink(UWidget *widget)
	:mColor(D2D1::ColorF::Black)
{
	mTargetView = widget;
	mBlinkCycler = new UCycler();

	mLocated = false;
	mCancelled = true;
	mBlinkMask = false;

	mThickness = UDeviceManager::dip(2);
}

UTextBlink::~UTextBlink()
{
	mCancelled = true;
	mBlinkCycler->removeCallbacks(this);
	delete mBlinkCycler;
}


void UTextBlink::draw(UCanvas *canvas)
{
	if (mBlinkMask && mLocated)
	{
		canvas->fillRect(D2D1::RectF(
			static_cast<float>(mBlinkRect.left),
			static_cast<float>(mBlinkRect.top),
			static_cast<float>(mBlinkRect.right),
			static_cast<float>(mBlinkRect.bottom)),
			mColor);
	}
}


void UTextBlink::show()
{
	if (!mLocated)
		return;

	mCancelled = false;
	mBlinkMask = false;

	mBlinkCycler->removeCallbacks(this);
	mBlinkCycler->post(this);
}

void UTextBlink::hide()
{
	mCancelled = true;
	mBlinkCycler->removeCallbacks(this);

	mBlinkMask = false;
	mTargetView->invalidate();
}

void UTextBlink::locate(float xCenter, float top, float bottom)
{
	int topInt = static_cast<int>(
		UMath::floor(top));
	int bottomInt = static_cast<int>(
		UMath::ceil(bottom));

	int leftInt = static_cast<int>(
		UMath::round(xCenter - mThickness / 2.f));
	int rightInt = static_cast<int>(
		leftInt + mThickness);

	//防止blink被截断。
	if (leftInt < 0)
	{
		rightInt += -leftInt;
		leftInt = 0;
	}

	if (topInt == mBlinkRect.top
		&& bottomInt == mBlinkRect.bottom
		&& leftInt == mBlinkRect.left
		&& rightInt == mBlinkRect.right)
		return;

	bool shouldShow = false;

	if (!mCancelled)
	{
		hide();
		shouldShow = true;
	}

	mBlinkRect.left = leftInt;
	mBlinkRect.top = topInt;
	mBlinkRect.right = rightInt;
	mBlinkRect.bottom = bottomInt;

	mLocated = true;

	if (shouldShow)
		show();
}


void UTextBlink::setColor(D2D1::ColorF color)
{
	mColor = color;
}

void UTextBlink::setThickness(float thickness)
{
	mThickness = thickness;
}


bool UTextBlink::isShowing()
{
	return !mCancelled;
}

D2D1::ColorF UTextBlink::getColor()
{
	return mColor;
}

float UTextBlink::getThickness()
{
	return mThickness;
}


void UTextBlink::run()
{
	if (!mCancelled)
	{
		mBlinkMask = !mBlinkMask;

		mTargetView->invalidate();

		mBlinkCycler->removeCallbacks(this);
		mBlinkCycler->postDelayed(this, 500);
	}
}