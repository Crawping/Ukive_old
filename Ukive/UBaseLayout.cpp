#include "UCommon.h"
#include "UMath.h"
#include "UWindow.h"
#include "ULayoutParams.h"
#include "BaseLayoutParams.h"
#include "UMenuImpl.h"
#include "UInputEvent.h"
#include "UInnerWindow.h"
#include "UBaseLayout.h"


UBaseLayout::UBaseLayout(UWindow *wnd)
	:UFrameLayout(wnd)
{
}

UBaseLayout::UBaseLayout(UWindow *wnd, int id)
	: UFrameLayout(wnd, id)
{
}


UBaseLayout::~UBaseLayout()
{
}


ULayoutParams *UBaseLayout::generateLayoutParams(
	ULayoutParams *lp)
{
	return new BaseLayoutParams(lp);
}

ULayoutParams *UBaseLayout::generateDefaultLayoutParams()
{
	return new BaseLayoutParams(
		BaseLayoutParams::FIT_CONTENT,
		BaseLayoutParams::FIT_CONTENT);
}

bool UBaseLayout::checkLayoutParams(ULayoutParams *lp)
{
	return typeid(*lp) == typeid(BaseLayoutParams);
}


bool UBaseLayout::hasContent()
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		BaseLayoutParams *lp
			= (BaseLayoutParams*)getChildAt(i)->getLayoutParams();
		if (lp->viewType == BaseLayoutParams::TYPE_NORMAL)
			return true;
	}

	return false;
}