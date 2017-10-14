#pragma once
#include "UWindow.h"
#include "OnClickListener.h"
#include "UContextMenuCallback.h"

class UContextMenu;

class TextWindow : public UWindow
{
private:
	static const int ID_TV_EDITOR = 0x1;
	static const int ID_LAYOUT_ROOT = 0x2;
	static const int ID_TOOLBAR = 0x3;
	static const int ID_TOOLBAR_ITEM_FONT = 0x10;
	static const int ID_TOOLBAR_ITEM_FORMAT = 0x11;

	class ToolbarMenuCallback : public UContextMenuCallback
	{
	private:
		TextWindow *mWindow;
	public:
		ToolbarMenuCallback(TextWindow *w) { mWindow = w; }

		bool onCreateContextMenu(UContextMenu *contextMenu, UMenu *menu) override;
		bool onPrepareContextMenu(UContextMenu *contextMenu, UMenu *menu) override;
		bool onContextMenuItemClicked(UContextMenu *contextMenu, UMenuItem *item) override;
		void onDestroyContextMenu(UContextMenu *contextMenu) override;
	};

	class FontItemClickListener : public OnClickListener
	{
	private:
		TextWindow *mWindow;
	public:
		FontItemClickListener(TextWindow *w) { mWindow = w; }
		void onClick(View *widget) override;
	};

	class FormatItemClickListener : public OnClickListener
	{
	private:
		TextWindow *mWindow;
	public:
		FormatItemClickListener(TextWindow *w) { mWindow = w; }
		void onClick(View *widget) override;
	};

private:
	UContextMenu *mContextMenu;
	ToolbarMenuCallback *mTBMCallback;

	View *inflateToolbar(View *parent);

public:
	TextWindow(UApplication *app);
	TextWindow(UApplication *app, int id);
	~TextWindow();

	virtual void onCreate() override;
	virtual bool onMoving(RECT *rect) override;
	virtual bool onResizing(WPARAM edge, RECT *rect) override;
	virtual void onMove(int x, int y) override;
	virtual void onResize(
		int param, int width, int height,
		int clientWidth, int clientHeight) override;
};