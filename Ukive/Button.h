#pragma once
#include "TextView.h"

class UShapeDrawable;
class URippleDrawable;

class Button : public TextView
{
private:
	UShapeDrawable *mShapeDrawable;
	URippleDrawable *mRippleBackground;

	void initButton();

public:
	Button(UWindow *wnd);
	Button(UWindow *wnd, int id);
	virtual ~Button();

	void setButtonColor(D2D1_COLOR_F color);
};