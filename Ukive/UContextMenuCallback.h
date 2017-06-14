#pragma once

class UMenu;
class UMenuItem;
class UContextMenu;

class UContextMenuCallback
{
public:
	virtual bool onCreateContextMenu(UContextMenu *contextMenu, UMenu *menu) = 0;
	virtual bool onPrepareContextMenu(UContextMenu *contextMenu, UMenu *menu) = 0;
	virtual bool onContextMenuItemClicked(UContextMenu *contextMenu, UMenuItem *item) = 0;
	virtual void onDestroyContextMenu(UContextMenu *contextMenu) = 0;
};