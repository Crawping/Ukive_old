#include "UCommon.h"
#include "URippleDrawable.h"
#include "UShapeDrawable.h"
#include "UInputEvent.h"
#include "UColor.h"
#include "Button.h"


Button::Button(UWindow *wnd)
	:TextView(wnd)
{
	initButton();
}

Button::Button(UWindow *wnd, int id)
	:TextView(wnd, id)
{
	initButton();
}


Button::~Button()
{
	delete mRippleBackground;
}


void Button::initButton()
{
	mShapeDrawable = new UShapeDrawable(mWindow, UShapeDrawable::SHAPE_ROUND_RECT);
	mShapeDrawable->setRadius(2.f);
	mShapeDrawable->setSolidEnable(true);
	mShapeDrawable->setSolidColor(UColor::White);

	mRippleBackground = new URippleDrawable(mWindow);
	mRippleBackground->addDrawable(mShapeDrawable);

	this->setText(L"UButton");
	this->setTextSize(13);
	this->setIsEditable(false);
	this->setIsSelectable(false);

	this->setPadding(24, 6, 24, 6);
	this->setBackground(mRippleBackground);
	this->setElevation(2.0f);
}

void Button::setButtonColor(D2D1_COLOR_F color)
{
	mShapeDrawable->setSolidColor(color);
}