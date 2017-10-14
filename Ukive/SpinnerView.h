#pragma once
#include "TextView.h"

class UWindow;

class SpinnerView : public TextView
{
public:
	SpinnerView(UWindow *wnd);
	SpinnerView(UWindow *wnd, int id);
	virtual ~SpinnerView();
};