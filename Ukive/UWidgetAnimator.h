#pragma once
#include "UAnimator.h"
#include "UTransition.h"

class UWidget;

class UWidgetAnimator : public UAnimator::OnValueChangedListener
{
public:
	static const int REVEAL_RECT = 0;
	static const int REVEAL_CIRCULE = 1;

private:
	UWidget *mOwningWidget;
	UAnimator *mAnimator;

	double mDuration;

	static const int UWIDGET_ANIM_X = 1;
	static const int UWIDGET_ANIM_Y = 2;
	static const int UWIDGET_ANIM_ALPHA = 3;
	static const int UWIDGET_ANIM_SCALE_X = 4;
	static const int UWIDGET_ANIM_SCALE_Y = 5;
	static const int UWIDGET_ANIM_TRANSLATE_X = 6;
	static const int UWIDGET_ANIM_TRANSLATE_Y = 7;
	static const int UWIDGET_ANIM_REVEAL = 8;

public:
	UWidgetAnimator(UWidget *widget);
	~UWidgetAnimator();

	void start();
	void cancel();
	UWidgetAnimator *setDuration(double duration);

	UWidgetAnimator *x(double x);
	UWidgetAnimator *y(double y);
	UWidgetAnimator *alpha(double value);
	UWidgetAnimator *scaleX(double value);
	UWidgetAnimator *scaleY(double value);
	UWidgetAnimator *translateX(double value);
	UWidgetAnimator *translateY(double value);

	UWidgetAnimator *setListener(UAnimator::OnAnimatorListener *l);

	static UAnimator *createRectReveal(
		UWidget *widget, double centerX, double centerY, 
		double startWidthRadius, double endWidthRadius, 
		double startHeightRadius, double endHeightRadius);
	static UAnimator *createCirculeReveal(
		UWidget *widget, double centerX, double centerY, double startRadius, double endRadius);

	void onValueChanged(
		unsigned int varIndex,
		IUIAnimationStoryboard *storyboard,
		IUIAnimationVariable *variable,
		double newValue, double previousValue) override;
	void onIntegerValueChanged(
		unsigned int varIndex,
		IUIAnimationStoryboard *storyboard,
		IUIAnimationVariable *variable,
		int newValue, int previousValue) override;
};