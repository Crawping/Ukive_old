#pragma once
#include "UExecutable.h"

class View;
class UCycler;

class TextBlink : public UExecutable
{
private:
	View *mTargetView;
	UCycler *mBlinkCycler;

	bool mLocated;
	bool mCancelled;
	bool mBlinkMask;
	
	URect mBlinkRect;

	float mThickness;
	D2D1::ColorF mColor;

public:
	TextBlink(View *widget);
	~TextBlink();

	void draw(UCanvas *canvas);

	void show();
	void hide();
	void locate(float xCenter, float top, float bottom);

	void setColor(D2D1::ColorF color);
	void setThickness(float thickness);

	bool isShowing();
	D2D1::ColorF getColor();
	float getThickness();

	void run();
};