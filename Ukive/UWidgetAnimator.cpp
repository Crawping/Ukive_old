#include "UCommon.h"
#include "UWidget.h"
#include "UWindow.h"
#include "UWidgetAnimator.h"


UWidgetAnimator::UWidgetAnimator(UWidget *widget)
{
	mOwningWidget = widget;
	mDuration = 0.2;

	mAnimator = new UAnimator(
		mOwningWidget->getWindow()->getAnimationManager());
}

UWidgetAnimator::~UWidgetAnimator()
{
	delete mAnimator;
}


void UWidgetAnimator::start()
{
	mAnimator->start();
}

void UWidgetAnimator::cancel()
{
	mAnimator->stop();
}

UWidgetAnimator *UWidgetAnimator::setDuration(double duration)
{
	mDuration = duration;

	return this;
}


UWidgetAnimator *UWidgetAnimator::x(float x)
{
	typedef std::numeric_limits<double> dLimit;

	mAnimator->addVariable(
		UWIDGET_ANIM_X, mOwningWidget->getX(), 
		-(dLimit::max)(), (dLimit::max)());
	mAnimator->addTransition(
		UWIDGET_ANIM_X, UTransition::linearTransition(mDuration, x));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_X, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::y(float y)
{
	typedef std::numeric_limits<double> dLimit;

	mAnimator->addVariable(
		UWIDGET_ANIM_Y, mOwningWidget->getY(), 
		-(dLimit::max)(), (dLimit::max)());
	mAnimator->addTransition(
		UWIDGET_ANIM_Y, UTransition::linearTransition(mDuration, y));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_Y, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::alpha(float value)
{
	mAnimator->addVariable(
		UWIDGET_ANIM_ALPHA, mOwningWidget->getAlpha(), 0, 1);
	mAnimator->addTransition(
		UWIDGET_ANIM_ALPHA, UTransition::linearTransition(mDuration, value));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_ALPHA, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::scaleX(float value)
{
	typedef std::numeric_limits<double> dLimit;

	mAnimator->addVariable(
		UWIDGET_ANIM_SCALE_X, mOwningWidget->getScaleX(), 
		-(dLimit::max)(), (dLimit::max)());
	mAnimator->addTransition(
		UWIDGET_ANIM_SCALE_X, UTransition::linearTransition(mDuration, value));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_SCALE_X, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::scaleY(float value)
{
	typedef std::numeric_limits<double> dLimit;

	mAnimator->addVariable(
		UWIDGET_ANIM_SCALE_Y, mOwningWidget->getScaleY(), 
		-(dLimit::max)(), (dLimit::max)());
	mAnimator->addTransition(
		UWIDGET_ANIM_SCALE_Y, UTransition::linearTransition(mDuration, value));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_SCALE_Y, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::translateX(float value)
{
	typedef std::numeric_limits<double> dLimit;

	mAnimator->addVariable(
		UWIDGET_ANIM_TRANSLATE_X, mOwningWidget->getTranslateX(), 
		-(dLimit::max)(), (dLimit::max)());
	mAnimator->addTransition(
		UWIDGET_ANIM_TRANSLATE_X, UTransition::linearTransition(mDuration, value));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_TRANSLATE_X, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::translateY(float value)
{
	typedef std::numeric_limits<double> dLimit;

	mAnimator->addVariable(
		UWIDGET_ANIM_TRANSLATE_Y, mOwningWidget->getTranslateY(), 
		-(dLimit::max)(), (dLimit::max)());
	mAnimator->addTransition(
		UWIDGET_ANIM_TRANSLATE_Y, UTransition::linearTransition(mDuration, value));
	mAnimator->setOnValueChangedListener(UWIDGET_ANIM_TRANSLATE_Y, this);

	return this;
}

UWidgetAnimator *UWidgetAnimator::setListener(UAnimator::OnAnimatorListener *l)
{
	mAnimator->setOnStateChangedListener(l);

	return this;
}


UAnimator *UWidgetAnimator::createRectReveal(
	UWidget *widget, float centerX, float centerY,
	float startWidthRadius, float endWidthRadius,
	float startHeightRadius, float endHeightRadius)
{
	typedef std::numeric_limits<double> dLimit;

	UAnimator *animator = new UAnimator(
		widget->getWindow()->getAnimationManager());
	animator->addVariable(0, startWidthRadius, -(dLimit::max)(), (dLimit::max)());
	animator->addTransition(0, UTransition::linearTransition(0.1, endWidthRadius));
	animator->addVariable(1, startHeightRadius, -(dLimit::max)(), (dLimit::max)());
	animator->addTransition(1, UTransition::linearTransition(0.2, endHeightRadius));

	widget->setHasReveal(true);
	widget->setRevealType(REVEAL_RECT);
	widget->setRevealCenterX(centerX);
	widget->setRevealCenterY(centerY);
	widget->setRevealWidthRadius(startWidthRadius);
	widget->setRevealHeightRadius(startHeightRadius);

	class RevealValueListener
		: public UAnimator::OnValueChangedListener
	{
	private:
		UWidget *mWidget;
	public:
		RevealValueListener(UWidget *w)
		{
			mWidget = w;
		}
		void onValueChanged(
			unsigned int varIndex,
			IUIAnimationStoryboard *storyboard,
			IUIAnimationVariable *variable,
			double newValue, double previousValue)
		{
			if (varIndex == 0)
				mWidget->setRevealWidthRadius(newValue);
			else if (varIndex == 1)
				mWidget->setRevealHeightRadius(newValue);
		}
		void onIntegerValueChanged(
			unsigned int varIndex,
			IUIAnimationStoryboard *storyboard,
			IUIAnimationVariable *variable,
			int newValue, int previousValue) {}
	}*valueListener = new RevealValueListener(widget);

	class RevealStateListener
		: public UAnimator::OnAnimatorListener
	{
	private:
		UWidget *mWidget;
	public:
		RevealStateListener(UWidget *w)
		{
			mWidget = w;
		}

		void onAnimationStart(UAnimator *animator)
		{
		}
		void onAnimationEnd(UAnimator *animator)
		{
			mWidget->setHasReveal(false);
		}
		void onAnimationCancel(UAnimator *animator)
		{
			mWidget->setHasReveal(false);
		}
	}*stateListener = new RevealStateListener(widget);

	animator->setOnValueChangedListener(0, valueListener);
	animator->setOnValueChangedListener(1, valueListener);
	animator->setOnStateChangedListener(stateListener);

	return animator;
}

UAnimator *UWidgetAnimator::createCirculeReveal(
	UWidget *widget, float centerX, float centerY, float startRadius, float endRadius)
{
	typedef std::numeric_limits<double> dLimit;

	UAnimator *animator = new UAnimator(
		widget->getWindow()->getAnimationManager());
	animator->addVariable(0, startRadius, -(dLimit::max)(), (dLimit::max)());
	animator->addTransition(0, UTransition::linearTransition(0.15, endRadius));

	widget->setHasReveal(true);
	widget->setRevealType(REVEAL_CIRCULE);
	widget->setRevealCenterX(centerX);
	widget->setRevealCenterY(centerY);
	widget->setRevealRadius(startRadius);

	class RevealValueListener
		: public UAnimator::OnValueChangedListener
	{
	private:
		UWidget *mWidget;
	public:
		RevealValueListener(UWidget *w)
		{
			mWidget = w;
		}
		void onValueChanged(
			unsigned int varIndex,
			IUIAnimationStoryboard *storyboard,
			IUIAnimationVariable *variable,
			double newValue, double previousValue)
		{
			mWidget->setRevealRadius(newValue);
		}
		void onIntegerValueChanged(
			unsigned int varIndex,
			IUIAnimationStoryboard *storyboard,
			IUIAnimationVariable *variable,
			int newValue, int previousValue) {}
	}*valueListener = new RevealValueListener(widget);

	class RevealStateListener
		: public UAnimator::OnAnimatorListener
	{
	private:
		UWidget *mWidget;
	public:
		RevealStateListener(UWidget *w)
		{
			mWidget = w;
		}

		void onAnimationStart(UAnimator *animator)
		{
		}
		void onAnimationEnd(UAnimator *animator)
		{
			mWidget->setHasReveal(false);
			//mWidget->invalidate();
		}
		void onAnimationCancel(UAnimator *animator)
		{
			mWidget->setHasReveal(false);
			//mWidget->invalidate();
		}
	}*stateListener = new RevealStateListener(widget);

	animator->setOnValueChangedListener(0, valueListener);
	animator->setOnStateChangedListener(stateListener);

	return animator;
}


void UWidgetAnimator::onValueChanged(
	unsigned int varIndex,
	IUIAnimationStoryboard *storyboard,
	IUIAnimationVariable *variable,
	double newValue, double previousValue)
{
	switch (varIndex)
	{
	case UWIDGET_ANIM_X:
		mOwningWidget->setX(newValue);
		break;
	case UWIDGET_ANIM_Y:
		mOwningWidget->setY(newValue);
		break;
	case UWIDGET_ANIM_ALPHA:
		mOwningWidget->setAlpha(newValue);
		break;
	case UWIDGET_ANIM_SCALE_X:
		mOwningWidget->setScaleX(newValue);
		break;
	case UWIDGET_ANIM_SCALE_Y:
		mOwningWidget->setScaleY(newValue);
		break;
	case UWIDGET_ANIM_TRANSLATE_X:
		mOwningWidget->setTranslateX(newValue);
		break;
	case UWIDGET_ANIM_TRANSLATE_Y:
		mOwningWidget->setTranslateY(newValue);
		break;
	case UWIDGET_ANIM_REVEAL:
		mOwningWidget->setRevealRadius(newValue);
		break;
	}
}

void UWidgetAnimator::onIntegerValueChanged(
	unsigned int varIndex,
	IUIAnimationStoryboard *storyboard,
	IUIAnimationVariable *variable,
	int newValue, int previousValue)
{
}