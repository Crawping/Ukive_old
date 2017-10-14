#include "UCommon.h"
#include "UColor.h"
#include "UColorDrawable.h"
#include "URippleDrawable.h"
#include "Button.h"
#include "TextView.h"
#include "LinearLayout.h"
#include "LinearLayoutParams.h"
#include "BaseLayoutParams.h"
#include "RestraintLayout.h"
#include "RestraintLayoutParams.h"
#include "OnClickListener.h"
#include "UContextMenu.h"
#include "TextWindow.h"


TextWindow::TextWindow(UApplication *app)
	:UWindow(app)
{
}

TextWindow::TextWindow(UApplication *app, int id)
	: UWindow(app, id)
{
}

TextWindow::~TextWindow()
{
}


void TextWindow::onCreate()
{
	UWindow::onCreate();

	typedef RestraintLayoutParams Rlp;

	mTBMCallback = new ToolbarMenuCallback(this);

	//root view.
	RestraintLayout *rootLayout = new RestraintLayout(this, ID_LAYOUT_ROOT);
	BaseLayoutParams *rightViewLp = new BaseLayoutParams(
		LayoutParams::MATCH_PARENT, LayoutParams::MATCH_PARENT);
	rootLayout->setLayoutParams(rightViewLp);

	setContentView(rootLayout);

	//Toolbar.
	/*auto toolbar = inflateToolbar(rootLayout);

	RestraintLayoutParams *toolbarLp = new RestraintLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	toolbarLp
		->startHandle(ID_LAYOUT_ROOT, RestraintLayoutParams::EDGE_START)
		->topHandle(ID_LAYOUT_ROOT, RestraintLayoutParams::EDGE_TOP)
		->endHandle(ID_LAYOUT_ROOT, RestraintLayoutParams::EDGE_END);

	rootLayout->addWidget(toolbar, toolbarLp);*/
	

	//编辑器。
	TextView *editorTV = new TextView(this, ID_TV_EDITOR);
	editorTV->setIsEditable(true);
	editorTV->setIsSelectable(true);
	editorTV->setFontFamilyName(L"Consolas");
	editorTV->setTextSize(15);
	editorTV->setPadding(18, 18, 18, 18);
	editorTV->setLineSpacing(true, 1.2f);
	editorTV->autoWrap(false);
	editorTV->requestFocus();

	Rlp *editorTVLp = Rlp::Builder(
		LayoutParams::MATCH_PARENT, LayoutParams::MATCH_PARENT)
		.start(ID_LAYOUT_ROOT, RestraintLayoutParams::START)
		.top(ID_LAYOUT_ROOT, RestraintLayoutParams::TOP)
		.end(ID_LAYOUT_ROOT, RestraintLayoutParams::END)
		.bottom(ID_LAYOUT_ROOT, RestraintLayoutParams::BOTTOM).build();

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
	UWindow::onMove(x, y);
}

void TextWindow::onResize(
	int param, int width, int height,
	int clientWidth, int clientHeight)
{
	::OutputDebugStringW(L"onResize()\n");
	UWindow::onResize(param, width, height, clientWidth, clientHeight);
}


View *TextWindow::inflateToolbar(View *parent)
{
	LinearLayout *toolbar = new LinearLayout(this, ID_TOOLBAR);
	toolbar->setElevation(1);
	toolbar->setOrientation(LinearLayout::HORIZONTAL);
	toolbar->setBackground(new UColorDrawable(this, UColor::White));

	TextView *font = new TextView(this, ID_TOOLBAR_ITEM_FONT);
	font->setText(L"字体");
	font->setTextSize(13);
	font->setPadding(16, 8, 16, 8);
	font->setBackground(new URippleDrawable(this));
	font->setOnClickListener(new FontItemClickListener(this));
	font->setFocusable(true);

	LinearLayoutParams *fontParams
		= new LinearLayoutParams(
			LinearLayoutParams::FIT_CONTENT, LinearLayoutParams::MATCH_PARENT);
	toolbar->addWidget(font, fontParams);

	TextView *format = new TextView(this, ID_TOOLBAR_ITEM_FORMAT);
	format->setText(L"格式");
	format->setTextSize(13);
	format->setPadding(16, 8, 16, 8);
	format->setBackground(new URippleDrawable(this));
	format->setOnClickListener(new FormatItemClickListener(this));
	format->setFocusable(true);

	LinearLayoutParams *formatParams
		= new LinearLayoutParams(
			LinearLayoutParams::FIT_CONTENT, LinearLayoutParams::MATCH_PARENT);
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


void TextWindow::FontItemClickListener::onClick(View *widget)
{
	if (mWindow->mContextMenu == nullptr)
	{
		mWindow->mContextMenu = mWindow->startContextMenu(
			mWindow->mTBMCallback, widget, Gravity::LEFT);

		mWindow->findWidgetById(TextWindow::ID_TOOLBAR_ITEM_FONT)->requestFocus();
	}
}

void TextWindow::FormatItemClickListener::onClick(View *widget)
{
	if (mWindow->mContextMenu == nullptr)
	{
		mWindow->mContextMenu = mWindow->startContextMenu(
			mWindow->mTBMCallback, widget, Gravity::LEFT);

		mWindow->findWidgetById(TextWindow::ID_TOOLBAR_ITEM_FORMAT)->requestFocus();
	}
}