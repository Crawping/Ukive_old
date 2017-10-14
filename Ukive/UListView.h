#pragma once
#include "ViewGroup.h"

class UWindow;
class UListAdapter;


class UListView : public ViewGroup
{
private:
	UListAdapter *mAdapter;

	void initListView();

public:
	UListView(UWindow *wnd);
	UListView(UWindow *wnd, int id);
	virtual ~UListView();

	void setAdapter(UListAdapter *adapter);
};