#include "UCommon.h"
#include "UWindow.h"
#include "UMath.h"
#include "UCanvas.h"
#include "UColor.h"
#include "UInputEvent.h"
#include "UTransition.h"
#include "UDeviceManager.h"
#include "SeekBar.h"


SeekBar::SeekBar(UWindow *window)
	:View(window)
{
	initSeekBar();
}

SeekBar::SeekBar(UWindow *window, int id)
	: View(window, id)
{
	initSeekBar();
}

SeekBar::~SeekBar()
{
	delete mThumbInAnimator;
	delete mThumbOutAnimator;
}


void SeekBar::initSeekBar()
{
	mMaximum = 100.f;
	mSeekPercent = 0.f;
	mIsMouseLeftKeyAvailable = false;
	mListener = nullptr;

	mThumbInAnimator = new UAnimator(mWindow->getAnimationManager());
	mThumbOutAnimator = new UAnimator(mWindow->getAnimationManager());

	mSeekTrackHeight = (int)UDeviceManager::dip(2);
	mSeekThumbMinDiameter = (int)UDeviceManager::dip(10);
	mSeekThumbMaxDiameter = (int)UDeviceManager::dip(14);

	mSeekThumbCurDiameter = mSeekThumbMinDiameter;
}


void SeekBar::setMaximum(float maximum)
{
	if (mMaximum > 0)
		mMaximum = maximum;
}

void SeekBar::setProgress(float progress, bool notify)
{
	float prog = UMath::minimum(mMaximum, progress);
	float percent = prog / mMaximum;
	if (percent != mSeekPercent)
	{
		mSeekPercent = percent;

		if (notify && mListener)
		{
			mListener->onSeekValueChanged(this, mSeekPercent*mMaximum);
			mListener->onSeekIntegerValueChanged(this, static_cast<int>(mSeekPercent*mMaximum));
		}

		invalidate();
	}
}

float SeekBar::getProgress()
{
	return mMaximum*mSeekPercent;
}

void SeekBar::setOnSeekValueChangedListener(OnSeekValueChangedListener *l)
{
	mListener = l;
}


bool SeekBar::isMouseInThumb(int mouseX, int mouseY)
{
	float thumbRadius = mSeekThumbMaxDiameter / 2.f;
	int trackWidth = getWidth() - mSeekThumbMaxDiameter - mPaddingLeft - mPaddingRight;

	float thumbCenterXInTrack = trackWidth*mSeekPercent + thumbRadius;
	float thumbCenterYInTrack = mPaddingTop + thumbRadius;

	if (((mouseX - mPaddingLeft) >= (thumbCenterXInTrack - thumbRadius))
		&& ((mouseX - mPaddingLeft) <= (thumbCenterXInTrack + thumbRadius)))
	{
		if ((mouseY - mPaddingTop) >= (thumbCenterYInTrack - thumbRadius)
			&& (mouseY - mPaddingTop) <= (thumbCenterYInTrack + thumbRadius))
			return true;
	}

	return false;
}

bool SeekBar::isMouseInTrack(int mouseX, int mouseY)
{
	float trackSpace = getWidth() - mPaddingLeft - mPaddingRight;

	if ((mouseX - mPaddingLeft) >= 0
		&& (mouseX - mPaddingLeft) <= trackSpace)
	{
		if ((mouseY - mPaddingTop) >= 0
			&& (mouseY - mPaddingTop) <= mSeekThumbMaxDiameter)
			return true;
	}

	return false;
}

void SeekBar::computePercent(int mouseX, int mouseY)
{
	float mouseInTrack = mouseX - mPaddingLeft - mSeekThumbMaxDiameter / 2.f;
	float trackWidth = getWidth() - mSeekThumbMaxDiameter - mPaddingLeft - mPaddingRight;
	mSeekPercent = UMath::maximum(0.f, mouseInTrack / trackWidth);
	mSeekPercent = UMath::minimum(1.f, mSeekPercent);

	if (mListener)
	{
		mListener->onSeekValueChanged(this, mSeekPercent*mMaximum);
		mListener->onSeekIntegerValueChanged(this, static_cast<int>(mSeekPercent*mMaximum));
	}
}

void SeekBar::startZoomInAnimation()
{
	if (mSeekThumbCurDiameter < mSeekThumbMaxDiameter)
	{
		mThumbInAnimator->stop();
		mThumbOutAnimator->stop();

		mThumbInAnimator->addVariable(0,
			mSeekThumbCurDiameter,
			mSeekThumbMinDiameter,
			mSeekThumbMaxDiameter);
		mThumbInAnimator->setOnValueChangedListener(0, this);
		mThumbInAnimator->startTransition(
			0, UTransition::linearTransition(0.1, mSeekThumbMaxDiameter));
	}
}

void SeekBar::startZoomOutAnimation()
{
	if (mSeekThumbCurDiameter > mSeekThumbMinDiameter)
	{
		mThumbInAnimator->stop();
		mThumbOutAnimator->stop();

		mThumbOutAnimator->addVariable(0,
			mSeekThumbCurDiameter,
			mSeekThumbMinDiameter,
			mSeekThumbMaxDiameter);
		mThumbOutAnimator->setOnValueChangedListener(0, this);
		mThumbOutAnimator->startTransition(
			0, UTransition::linearTransition(0.1, mSeekThumbMinDiameter));
	}
}


void SeekBar::onMeasure(
	int width, int height,
	int widthMode, int heightMode)
{
	std::uint32_t finalWidth = 0;
	std::uint32_t finalHeight = 0;

	int hPadding = mPaddingLeft + mPaddingRight;
	int vPadding = mPaddingTop + mPaddingBottom;

	switch (widthMode)
	{
	case EXACTLY:
		finalWidth = width;
		break;

	case FIT:
		finalWidth = UMath::maximum(width, getMinimumWidth());
		break;

	case UNKNOWN:
		finalWidth = UMath::maximum(0, getMinimumWidth());
		break;
	}

	switch (heightMode)
	{
	case EXACTLY:
		finalHeight = height;
		break;

	case FIT:
		finalHeight = UMath::minimum(height, mSeekThumbMaxDiameter + vPadding);
		finalHeight = UMath::maximum(finalHeight, getMinimumHeight());
		break;

	case UNKNOWN:
		finalHeight = UMath::maximum(mSeekThumbMaxDiameter + vPadding, getMinimumHeight());
		break;
	}

	setMeasuredDimension(finalWidth, finalHeight);
}

void SeekBar::onDraw(UCanvas *canvas)
{
	float left = mSeekThumbMaxDiameter / 2.f;
	float top = mSeekThumbMaxDiameter / 2.f - mSeekTrackHeight / 2.f;
	float trackWidth = getWidth() - mSeekThumbMaxDiameter - mPaddingLeft - mPaddingRight;

	float centerX = left + trackWidth*mSeekPercent;
	float centerY = mSeekThumbMaxDiameter / 2.f;

	if (centerX < mSeekThumbMinDiameter)
		canvas->fillRect(D2D1::RectF(left, top, left + trackWidth, top + mSeekTrackHeight), UColor::Grey300);
	else
	{
		canvas->fillRect(D2D1::RectF(left, top, centerX, top + mSeekTrackHeight), UColor::Blue400);
		canvas->fillRect(D2D1::RectF(centerX, top, left + trackWidth, top + mSeekTrackHeight), UColor::Grey300);
	}

	canvas->fillCircle(centerX, centerY, mSeekThumbCurDiameter / 2.f, UColor::Blue400);
}

bool SeekBar::onInputEvent(UInputEvent *e)
{
	View::onInputEvent(e);

	switch (e->getEvent())
	{
	case UInputEvent::EVENT_MOUSE_LEAVE:
	{
		startZoomOutAnimation();
		break;
	}

	case UInputEvent::EVENT_MOUSE_KEY_DOWN:
	{
		if (e->getMouseKey() == UInputEvent::KEY_MOUSE_LEFT)
		{
			if (isMouseInThumb(e->getMouseX(), e->getMouseY()))
			{
				mIsMouseLeftKeyAvailable = true;
				computePercent(e->getMouseX(), e->getMouseY());
				invalidate();
			}
			else if (isMouseInTrack(e->getMouseX(), e->getMouseY()))
			{
				mIsMouseLeftKeyAvailable = true;
				computePercent(e->getMouseX(), e->getMouseY());
				startZoomInAnimation();
			}
		}

		break;
	}

	case UInputEvent::EVENT_MOUSE_MOVE:
	{
		if (mIsMouseLeftKeyAvailable)
		{
			computePercent(e->getMouseX(), e->getMouseY());
			invalidate();
		}
		else
		{
			if (isMouseInThumb(e->getMouseX(), e->getMouseY()))
				startZoomInAnimation();
			else
				startZoomOutAnimation();
		}
		break;
	}

	case UInputEvent::EVENT_MOUSE_KEY_UP:
	{
		if (e->getMouseKey() == UInputEvent::KEY_MOUSE_LEFT)
		{
			if (mIsMouseLeftKeyAvailable)
			{
				mIsMouseLeftKeyAvailable = false;
			}
		}
		break;
	}
	}

	return true;
}


void SeekBar::onValueChanged(
	unsigned int varIndex,
	IUIAnimationStoryboard *storyboard,
	IUIAnimationVariable *variable,
	double newValue, double previousValue)
{
	mSeekThumbCurDiameter = newValue;
}

void SeekBar::onIntegerValueChanged(
	unsigned int varIndex,
	IUIAnimationStoryboard *storyboard,
	IUIAnimationVariable *variable,
	int newValue, int previousValue)
{
}