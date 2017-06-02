#pragma once
#include "UWidgetGroup.h"

class UWindow;

class UListView : public UWidgetGroup
{
private:
	void initListView();

public:
	UListView(UWindow *wnd);
	UListView(UWindow *wnd, int id);
	virtual ~UListView();
};