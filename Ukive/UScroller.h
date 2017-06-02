#pragma once

class UWindow;
class UAnimator;

class UScroller
{
private:
	UAnimator *mScrollAnimator;

public:
	UScroller(UWindow *wnd);
	~UScroller();

	void fling(int startX, int startY, float velocityX, float velocityY);
	void startScroll(int startX, int startY, int dx, int dy, double duration);
};