#pragma once
#include "UDrawable.h"

class UEditTextDrawable : public UDrawable
{
private:
	float mPanelLineWidth;
	D2D1_COLOR_F mPanelLineColor;

public:
	UEditTextDrawable(UWindow *wnd);
	~UEditTextDrawable();

	bool onFocusChanged(bool focus) override;

	void draw(UCanvas *canvas) override;

	float getOpacity() override;
};