#include "UCommon.h"
#include "USwitchView.h"


USwitchView::USwitchView(UWindow *wnd)
	:UWidget(wnd)
{
	initSwitchView();
}

USwitchView::USwitchView(UWindow *wnd, int id)
	: UWidget(wnd, id)
{
	initSwitchView();
}

USwitchView::~USwitchView()
{
}


void USwitchView::initSwitchView()
{
}