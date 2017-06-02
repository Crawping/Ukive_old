#pragma once
#include "UFrameLayout.h"
#include "UMenu.h"

class UWindow;
class ULayoutParams;
class BaseLayoutParams;
class UInnerWindow;

class UBaseLayout : public UFrameLayout
{
protected:
	virtual ULayoutParams *generateLayoutParams(ULayoutParams *lp) override;
	virtual ULayoutParams *generateDefaultLayoutParams() override;
	virtual bool checkLayoutParams(ULayoutParams *lp) override;

public:
	UBaseLayout(UWindow *wnd);
	UBaseLayout(UWindow *wnd, int id);
	~UBaseLayout();

	bool hasContent();
};