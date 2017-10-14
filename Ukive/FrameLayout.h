#pragma once
#include "ViewGroup.h"

class FrameLayout : public ViewGroup
{
public:
	FrameLayout(UWindow *wnd);
	FrameLayout(UWindow *wnd, int id);
	virtual ~FrameLayout();

	virtual void onMeasure(
		int width, int height, int widthSpec, int heightSpec) override;
	virtual void onLayout(
		bool changed, bool sizeChanged,
		int left, int top, int right, int bottom) override;
};