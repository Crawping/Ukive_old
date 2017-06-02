#include "UCommon.h"
#include "UAnimator.h"
#include "UTransition.h"
#include "UWindow.h"
#include "UScroller.h"


UScroller::UScroller(UWindow *wnd)
{
	mScrollAnimator = new UAnimator(wnd->getAnimationManager());
}

UScroller::~UScroller()
{
	mScrollAnimator->stop();
	delete mScrollAnimator;
}


void UScroller::fling(int startX, int startY, float velocityX, float velocityY)
{
	mScrollAnimator->reset();
	mScrollAnimator->addVariable(0, startX, INT32_MIN, INT32_MAX);
	mScrollAnimator->addVariable(1, startY, INT32_MIN, INT32_MAX);
	//mScrollAnimator->addTransition(0, UTransition::acc());
	//mScrollAnimator->addTransition(1, UTransition::linearTransition(duration, startY + dy));
	mScrollAnimator->start();
}

void UScroller::startScroll(int startX, int startY, int dx, int dy, double duration)
{
	mScrollAnimator->reset();
	mScrollAnimator->addVariable(0, startX, INT32_MIN, INT32_MAX);
	mScrollAnimator->addVariable(1, startY, INT32_MIN, INT32_MAX);
	mScrollAnimator->addTransition(0, UTransition::linearTransition(duration, startX + dx));
	mScrollAnimator->addTransition(1, UTransition::linearTransition(duration, startY + dy));
	mScrollAnimator->start();
}