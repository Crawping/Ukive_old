#pragma once
#include "UDrawable.h"

class UColorDrawable : public UDrawable
{
private:
	D2D1_COLOR_F mColor;

public:
	UColorDrawable(UWindow *wnd, D2D1_COLOR_F color);
	~UColorDrawable();

	virtual void draw(UCanvas *canvas) override;
	virtual float getOpacity() override;

	D2D1_COLOR_F getColor();
};