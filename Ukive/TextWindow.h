#pragma once
#include "UWindow.h"
#include "OnClickListener.h"
#include "UContextMenuCallback.h"

class UContextMenu;

class TextWindow : public UWindow
{
private:
	static const uint32_t ID_TV_EDITOR = 0x1;
	static const uint32_t ID_LAYOUT_ROOT = 0x2;
	static const uint32_t ID_TOOLBAR = 0x3;
	static const uint32_t ID_TOOLBAR_ITEM_FONT = 0x10;
	static const uint32_t ID_TOOLBAR_ITEM_FORMAT = 0x11;

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
		void onClick(UWidget *widget) override;
	};

	class FormatItemClickListener : public OnClickListener
	{
	private:
		TextWindow *mWindow;
	public:
		FormatItemClickListener(TextWindow *w) { mWindow = w; }
		void onClick(UWidget *widget) override;
	};

private:
	UContextMenu *mContextMenu;
	ToolbarMenuCallback *mTBMCallback;

	UWidget *inflateToolbar(UWidget *parent);

public:
	//使用基类的构造函数。
	using UWindow::UWindow;

	virtual void onCreate() override;
	virtual bool onMoving(RECT *rect) override;
	virtual bool onResizing(WPARAM edge, RECT *rect) override;
	virtual void onMove(int x, int y) override;
	virtual void onResize(
		int param, int width, int height,
		int clientWidth, int clientHeight) override;
};