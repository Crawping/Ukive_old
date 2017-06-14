#include "UCommon.h"
#include "UColor.h"
#include "UColorDrawable.h"
#include "URippleDrawable.h"
#include "UButton.h"
#include "UTextView.h"
#include "ULinearLayout.h"
#include "ULinearLayoutParams.h"
#include "RestrainLayout.h"
#include "UBaseLayoutParams.h"
#include "RestrainLayoutParams.h"
#include "OnClickListener.h"
#include "UContextMenu.h"
#include "TextWindow.h"


void TextWindow::onCreate()
{
	UWindow::onCreate();

	mTBMCallback = new ToolbarMenuCallback(this);

	//root view.
	RestrainLayout *rootLayout = new RestrainLayout(this, ID_LAYOUT_ROOT);
	UBaseLayoutParams *rightViewLp = new UBaseLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::MATCH_PARENT);
	rootLayout->setLayoutParams(rightViewLp);

	setContentView(rootLayout);

	//Toolbar.
	auto toolbar = inflateToolbar(rootLayout);

	RestrainLayoutParams *toolbarLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	toolbarLp->startHandledId = ID_LAYOUT_ROOT;
	toolbarLp->startHandledEdge = RestrainLayoutParams::EDGE_START;
	toolbarLp->leftMargin = 0;
	toolbarLp->topHandledId = ID_LAYOUT_ROOT;
	toolbarLp->topHandledEdge = RestrainLayoutParams::EDGE_TOP;
	toolbarLp->topMargin = 0;
	toolbarLp->endHandledId = ID_LAYOUT_ROOT;
	toolbarLp->endHandledEdge = RestrainLayoutParams::EDGE_END;
	toolbarLp->rightMargin = 0;
	//toolbarLp->bottomHandledId = ID_LAYOUT_ROOT;
	//toolbarLp->bottomHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	//toolbarLp->bottomMargin = 0;

	rootLayout->addWidget(toolbar, toolbarLp);
	

	//编辑器。
	UTextView *editorTV = new UTextView(this, ID_TV_EDITOR);
	editorTV->setIsEditable(true);
	editorTV->setIsSelectable(true);
	editorTV->setFontFamilyName(L"Consolas");
	editorTV->setTextSize(15);
	editorTV->setPadding(18, 18, 18, 18);
	//editorTV->setBackground(new UColorDrawable(this, UColor::Grey100));
	editorTV->setLineSpacing(true, 1.2f);
	editorTV->autoWrap(false);
	editorTV->requestFocus();

	RestrainLayoutParams *editorTVLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::MATCH_PARENT);
	editorTVLp->startHandledId = ID_LAYOUT_ROOT;
	editorTVLp->startHandledEdge = RestrainLayoutParams::EDGE_START;
	editorTVLp->leftMargin = 0;
	editorTVLp->topHandledId = ID_TOOLBAR;
	editorTVLp->topHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	editorTVLp->topMargin = 0;
	editorTVLp->endHandledId = ID_LAYOUT_ROOT;
	editorTVLp->endHandledEdge = RestrainLayoutParams::EDGE_END;
	editorTVLp->rightMargin = 0;
	editorTVLp->bottomHandledId = ID_LAYOUT_ROOT;
	editorTVLp->bottomHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	editorTVLp->bottomMargin = 0;

	rootLayout->addWidget(editorTV, editorTVLp);
}


bool TextWindow::onMoving(RECT *rect)
{
	::OutputDebugStringW(L"onMoving()\n");
	return UWindow::onMoving(rect);
}

bool TextWindow::onResizing(WPARAM edge, RECT *rect)
{
	::OutputDebugStringW(L"onResizing()\n");
	return UWindow::onResizing(edge, rect);
}

void TextWindow::onMove(int x, int y)
{
	::OutputDebugStringW(L"onMove()\n");
	return UWindow::onMove(x, y);
}

void TextWindow::onResize(
	int param, int width, int height,
	int clientWidth, int clientHeight)
{
	::OutputDebugStringW(L"onResize()\n");
	return UWindow::onResize(param, width, height, clientWidth, clientHeight);
}


UWidget *TextWindow::inflateToolbar(UWidget *parent)
{
	ULinearLayout *toolbar = new ULinearLayout(this, ID_TOOLBAR);
	toolbar->setElevation(1);
	toolbar->setOrientation(ULinearLayout::HORIZONTAL);
	toolbar->setBackground(new UColorDrawable(this, UColor::White));

	UTextView *font = new UTextView(this, ID_TOOLBAR_ITEM_FONT);
	font->setText(L"字体");
	font->setTextSize(13);
	font->setPadding(16, 8, 16, 8);
	font->setBackground(new URippleDrawable(this));
	font->setOnClickListener(new FontItemClickListener(this));
	font->setFocusable(true);

	ULinearLayoutParams *fontParams
		= new ULinearLayoutParams(
			ULinearLayoutParams::FIT_CONTENT, ULinearLayoutParams::MATCH_PARENT);
	toolbar->addWidget(font, fontParams);

	UTextView *format = new UTextView(this, ID_TOOLBAR_ITEM_FORMAT);
	format->setText(L"格式");
	format->setTextSize(13);
	format->setPadding(16, 8, 16, 8);
	format->setBackground(new URippleDrawable(this));
	format->setOnClickListener(new FormatItemClickListener(this));
	format->setFocusable(true);

	ULinearLayoutParams *formatParams
		= new ULinearLayoutParams(
			ULinearLayoutParams::FIT_CONTENT, ULinearLayoutParams::MATCH_PARENT);
	toolbar->addWidget(format, formatParams);

	return toolbar;
}


bool TextWindow::ToolbarMenuCallback::onCreateContextMenu(
	UContextMenu *contextMenu, UMenu *menu)
{
	menu->addItem(0, 0, L"Test");
	menu->addItem(1, 0, L"Test");
	menu->addItem(2, 0, L"Test");
	menu->addItem(3, 0, L"Test");

	return true;
}

bool TextWindow::ToolbarMenuCallback::onPrepareContextMenu(
	UContextMenu *contextMenu, UMenu *menu)
{
	return true;
}

bool TextWindow::ToolbarMenuCallback::onContextMenuItemClicked(
	UContextMenu *contextMenu, UMenuItem *item)
{
	contextMenu->close();
	return false;
}

void TextWindow::ToolbarMenuCallback::onDestroyContextMenu(
	UContextMenu *contextMenu)
{
	mWindow->mContextMenu = nullptr;
}


void TextWindow::FontItemClickListener::onClick(UWidget *widget)
{
	if (mWindow->mContextMenu == nullptr)
	{
		mWindow->mContextMenu = mWindow->startContextMenu(
			mWindow->mTBMCallback, widget, UGravity::LEFT);

		mWindow->findWidgetById(TextWindow::ID_TOOLBAR_ITEM_FONT)->requestFocus();
	}
}

void TextWindow::FormatItemClickListener::onClick(UWidget *widget)
{
	if (mWindow->mContextMenu == nullptr)
	{
		mWindow->mContextMenu = mWindow->startContextMenu(
			mWindow->mTBMCallback, widget, UGravity::LEFT);

		mWindow->findWidgetById(TextWindow::ID_TOOLBAR_ITEM_FORMAT)->requestFocus();
	}
}