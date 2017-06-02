#pragma once
#include "UTextView.h"

class UWindow;

class USpinnerView : public UTextView
{
public:
	USpinnerView(UWindow *wnd);
	USpinnerView(UWindow *wnd, int id);
	virtual ~USpinnerView();
};