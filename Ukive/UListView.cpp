#include "UCommon.h"
#include "UListView.h"


UListView::UListView(UWindow *wnd)
	:UWidgetGroup(wnd)
{
	initListView();
}

UListView::UListView(UWindow *wnd, int id)
	: UWidgetGroup(wnd, id)
{
	initListView();
}

UListView::~UListView()
{
}


void UListView::initListView()
{

}