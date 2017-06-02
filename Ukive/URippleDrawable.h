#pragma once
#include "ULayerDrawable.h"
#include "UAnimator.h"

#define DOWN_UP_SEC      0.2
#define HOVER_LEAVE_SEC  0.2

#define HOVER_ALPHA  0.07
#define DOWN_ALPHA   0.13

class UAnimator;

class URippleDrawable 
	: public ULayerDrawable, public UAnimator::OnValueChangedListener
{
private:
	double mAlpha;
	D2D1_COLOR_F mTintColor;

	UAnimator *mUpAnimator;
	UAnimator *mDownAnimator;
	
	UAnimator *mHoverAnimator;
	UAnimator *mLeaveAnimator;

	UAnimator *mRippleAnimator;

protected:
	bool onStateChanged(int newState, int prevState) override;

public:
	URippleDrawable(UWindow *wnd);
	~URippleDrawable();

	void setTintColor(D2D1_COLOR_F tint);

	void draw(UCanvas *canvas) override;

	float getOpacity() override;

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