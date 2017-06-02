#pragma once
#include "UWidget.h"

class UWindow;

class USwitchView : public UWidget
{
private:
	void initSwitchView();

public:
	USwitchView(UWindow *wnd);
	USwitchView(UWindow *wnd, int id);
	~USwitchView();
};