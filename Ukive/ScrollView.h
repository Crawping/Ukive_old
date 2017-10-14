#pragma once
#include "ViewGroup.h"

class UScroller;
class UInputEvent;

class ScrollView : public ViewGroup
{
private:
	int mMouseXCache;
	int mMouseYCache;

	UScroller *mScroller;

	bool canScroll();
	int computeScrollRange();
	int computeScrollExtend();

	void processVerticalScroll(int dy);

public:
	ScrollView(UWindow *wnd);
	ScrollView(UWindow *wnd, int id);
	virtual ~ScrollView();

	virtual void onMeasure(
		int width, int height,
		int widthSpec, int heightSpec) override;

	virtual void onLayout(
		bool changed, bool sizeChanged,
		int left, int top, int right, int bottom) override;

	virtual void onSizeChanged(
		int width, int height, int oldWidth, int oldHeight) override;

	virtual void onScrollChanged(
		int scrollX, int scrollY, int oldScrollX, int oldScrollY) override;

	virtual bool onInputEvent(UInputEvent *e) override;
	virtual bool onInterceptMouseEvent(UInputEvent *e) override;
};