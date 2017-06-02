#pragma once

class UMenu;
class UMenuItem;
class TextActionMode;

class TextActionModeCallback
{
public:
	virtual bool onCreateActionMode(TextActionMode *mode, UMenu *menu) = 0;
	virtual bool onPrepareActionMode(TextActionMode *mode, UMenu *menu) = 0;
	virtual bool onActionItemClicked(TextActionMode *mode, UMenuItem *item) = 0;
	virtual void onDestroyActionMode(TextActionMode *mode) = 0;
	virtual void onGetContentPosition(int *x, int *y) = 0;
};