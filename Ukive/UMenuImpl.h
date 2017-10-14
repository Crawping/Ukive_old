#pragma once
#include "LinearLayout.h"
#include "UMenu.h"
#include "OnClickListener.h"
#include "UMenuCallback.h"

class UWindow;
class UMenuCallback;
class View;

class UMenuImpl : public LinearLayout, public UMenu, public OnClickListener
{
private:
	int mItemHeight;
	UMenuCallback *mCallback;

	void initMenu();

public:
	UMenuImpl(UWindow *wnd);
	UMenuImpl(UWindow *wnd, int id);
	virtual ~UMenuImpl();

	void setMenuItemHeight(int height);

	void setCallback(UMenuCallback *callback) override;
	UMenuCallback *getCallback() override;

	UMenuItem *addItem(int id, std::int32_t order, std::wstring title) override;
	bool removeItem(int id) override;
	bool hasItem(int id) override;
	UMenuItem *findItem(int id) override;
	std::size_t getItemCount() override;

	void onClick(View *widget) override;
};

