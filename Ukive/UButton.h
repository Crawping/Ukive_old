#pragma once
#include "UTextView.h"

class UShapeDrawable;
class URippleDrawable;

class UButton : public UTextView
{
private:
	UShapeDrawable *mShapeDrawable;
	URippleDrawable *mRippleBackground;

	void initButton();

public:
	UButton(UWindow *wnd);
	UButton(UWindow *wnd, int id);
	virtual ~UButton();

	void setButtonColor(D2D1_COLOR_F color);
};