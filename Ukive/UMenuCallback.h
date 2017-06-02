#pragma once

class UMenu;
class UMenuItem;

class UMenuCallback
{
public:
	virtual void onCreateMenu(UMenu *menu) = 0;
	virtual void onPrepareMenu(UMenu *menu) = 0;
	virtual bool onMenuItemClicked(UMenu *menu, UMenuItem *item) = 0;
};