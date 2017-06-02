#include "UCommon.h"
#include "UColor.h"
#include "UColorDrawable.h"
#include "UButton.h"
#include "UTextView.h"
#include "RestrainLayout.h"
#include "BaseLayoutParams.h"
#include "RestrainLayoutParams.h"
#include "OnClickListener.h"
#include "TextWindow.h"


void TextWindow::onCreate()
{
	UWindow::onCreate();

	//root view.
	RestrainLayout *rootLayout = new RestrainLayout(this, ID_LAYOUT_ROOT);
	BaseLayoutParams *rightViewLp = new BaseLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::MATCH_PARENT);
	rootLayout->setLayoutParams(rightViewLp);

	setContentView(rootLayout);

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
	editorTVLp->topHandledId = ID_LAYOUT_ROOT;
	editorTVLp->topHandledEdge = RestrainLayoutParams::EDGE_TOP;
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