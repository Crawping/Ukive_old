#include "UCommon.h"
#include "SwitchView.h"


SwitchView::SwitchView(UWindow *wnd)
	:View(wnd)
{
	initSwitchView();
}

SwitchView::SwitchView(UWindow *wnd, int id)
	: View(wnd, id)
{
	initSwitchView();
}

SwitchView::~SwitchView()
{
}


void SwitchView::initSwitchView()
{
}