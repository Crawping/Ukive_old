#pragma once
#include "UMenuCallback.h"
#include "UInnerWindow.h"

class UWindow;
class UMenu;
class UMenuImpl;
class UMenuItem;
class TextActionModeCallback;

class TextActionMode : public UMenuCallback
{
private:
	int mMenuWidth;
	int mMenuItemHeight;
	bool mIsFinished;

	UWindow *mWindow;
	TextActionModeCallback *mCallback;

	UMenuImpl *mMenu;
	std::shared_ptr<UInnerWindow> mInnerWindow;

public:
	TextActionMode(UWindow *window, TextActionModeCallback *callback);
	~TextActionMode();

	void onCreateMenu(UMenu *menu) override;
	void onPrepareMenu(UMenu *menu) override;
	bool onMenuItemClicked(UMenu *menu, UMenuItem *item) override;

	UMenu *getMenu();

	void invalidateMenu();
	void invalidatePosition();

	void show();
	void close();
};