#pragma once
#include "UWidgetGroup.h"

class UFrameLayout : public UWidgetGroup
{
public:
	UFrameLayout(UWindow *wnd);
	UFrameLayout(UWindow *wnd, int id);
	virtual ~UFrameLayout();

	virtual void onMeasure(
		int width, int height, int widthSpec, int heightSpec) override;
	virtual void onLayout(
		bool changed, bool sizeChanged,
		int left, int top, int right, int bottom) override;
};