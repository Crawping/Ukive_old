#pragma once
#include "View.h"

class UWindow;

class SwitchView : public View
{
private:
	void initSwitchView();

public:
	SwitchView(UWindow *wnd);
	SwitchView(UWindow *wnd, int id);
	~SwitchView();
};