#pragma once
#include "UMenuCallback.h"
#include "UInnerWindow.h"

class UWindow;
class UMenu;
class UMenuImpl;
class UMenuItem;
class UContextMenuCallback;

class UContextMenu : public UMenuCallback
{
private:
	int mMenuWidth;
	int mMenuItemHeight;
	bool mIsFinished;

	UWindow *mWindow;
	UContextMenuCallback *mCallback;

	UMenuImpl *mMenu;
	std::shared_ptr<UInnerWindow> mInnerWindow;

public:
	UContextMenu(UWindow *window, UContextMenuCallback *callback);
	~UContextMenu();

	void onCreateMenu(UMenu *menu) override;
	void onPrepareMenu(UMenu *menu) override;
	bool onMenuItemClicked(UMenu *menu, UMenuItem *item) override;

	UMenu *getMenu();

	void invalidateMenu();

	void show(int x, int y);
	void close();
};