#include "UCommon.h"
#include "UListAdapter.h"
#include "UListView.h"


UListView::UListView(UWindow *wnd)
	:ViewGroup(wnd)
{
	initListView();
}

UListView::UListView(UWindow *wnd, int id)
	: ViewGroup(wnd, id)
{
	initListView();
}

UListView::~UListView()
{
}


void UListView::initListView()
{
	mAdapter = nullptr;
}


void UListView::setAdapter(UListAdapter *adapter)
{
	if (mAdapter != nullptr)
	{
	}

	mAdapter = adapter;
}