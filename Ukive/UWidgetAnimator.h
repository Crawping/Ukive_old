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

	UWidgetAnimator *x(float x);
	UWidgetAnimator *y(float y);
	UWidgetAnimator *alpha(float value);
	UWidgetAnimator *scaleX(float value);
	UWidgetAnimator *scaleY(float value);
	UWidgetAnimator *translateX(float value);
	UWidgetAnimator *translateY(float value);

	UWidgetAnimator *setListener(UAnimator::OnAnimatorListener *l);

	static UAnimator *createRectReveal(
		UWidget *widget, float centerX, float centerY, 
		float startWidthRadius, float endWidthRadius, 
		float startHeightRadius, float endHeightRadius);
	static UAnimator *createCirculeReveal(
		UWidget *widget, float centerX, float centerY, float startRadius, float endRadius);

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