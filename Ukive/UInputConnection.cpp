#include "UCommon.h"
#include "UWindow.h"
#include "UTextView.h"
#include "UEditable.h"
#include "UTsfEditor.h"
#include "UTsfManager.h"
#include "UInputConnection.h"


UInputConnection::UInputConnection(UTextView *textView)
{
	mTextView = textView;

	mTsfEditor = nullptr;
	mIsInitialized = false;
	mIsEditorPushed = false;
}

UInputConnection::~UInputConnection()
{
}


HRESULT UInputConnection::initialization(UTsfManager *tsfMgr)
{
	if (mIsInitialized)
		return S_OK;

	mTsfEditor = new UTsfEditor();
	mTsfEditor->setInputConnection(this);

	RH(tsfMgr->getThreadManager()->CreateDocumentMgr(&mDocumentMgr));

	RH(mDocumentMgr->CreateContext(tsfMgr->getClientId(), 0,
		dynamic_cast<ITextStoreACP*>(mTsfEditor),
		&mEditorContext, &mEditorCookie));

	RH(mEditorContext->QueryInterface(
		IID_ITfContextOwnerCompositionServices, (void**)&mCompServices));

	mIsInitialized = true;

	return S_OK;
}

void UInputConnection::pushEditor()
{
	if (!mIsInitialized || mIsEditorPushed)
		return;

	mDocumentMgr->Push(mEditorContext.get());

	mIsEditorPushed = true;
}

void UInputConnection::popEditor()
{
	if (!mIsInitialized || !mIsEditorPushed)
		return;

	mDocumentMgr->Pop(TF_POPF_ALL);

	mIsEditorPushed = false;
}

bool UInputConnection::mount(UTsfManager *tsfMgr)
{
	if (!mIsInitialized)
		return false;

	HRESULT hr = tsfMgr->getThreadManager()->SetFocus(mDocumentMgr.get());
	if (FAILED(hr))
		return false;

	return true;
}

bool UInputConnection::unmount(UTsfManager *tsfMgr)
{
	if (!mIsInitialized)
		return false;

	HRESULT hr = tsfMgr->getThreadManager()->SetFocus(nullptr);
	if (FAILED(hr))
		return false;

	return true;
}

bool UInputConnection::terminateComposition()
{
	if (!mIsInitialized)
		return false;

	HRESULT hr = mCompServices->TerminateComposition(nullptr);
	if (FAILED(hr))
		return false;

	return true;
}


void UInputConnection::notifyStatusChanged(DWORD flags)
{
	mTsfEditor->notifyStatusChanged(flags);
}

void UInputConnection::notifyTextChanged(bool correction, long start, long oldEnd, long newEnd)
{
	mTsfEditor->notifyTextChanged(correction, start, oldEnd, newEnd);
}

void UInputConnection::notifyTextLayoutChanged(TsLayoutCode reason)
{
	mTsfEditor->notifyTextLayoutChanged(reason);
}

void UInputConnection::notifyTextSelectionChanged()
{
	mTsfEditor->notifyTextSelectionChanged();
}


void UInputConnection::onBeginProcess()
{
	mTextView->onBeginProcess();
}

void UInputConnection::onEndProcess()
{
	mTextView->onEndProcess();
}

bool UInputConnection::isReadOnly()
{
	return !mTextView->isEditable();
}

void UInputConnection::determineInsert(
	long start, long end, unsigned long repLength,
	long *resStart, long *resEnd)
{
	*resStart = start;
	*resEnd = end;
}

bool UInputConnection::getSelection(
	unsigned long startIndex, unsigned long maxCount,
	TS_SELECTION_ACP *selections, unsigned long *fetchedCount)
{
	if (startIndex != TF_DEFAULT_SELECTION || maxCount != 1)
		return false;
	
	int selStart = mTextView->getSelectionStart();
	int selEnd = mTextView->getSelectionEnd();

	selections[0].acpStart = selStart;
	selections[0].acpEnd = selEnd;
	selections[0].style.fInterimChar = FALSE;
	selections[0].style.ase = TS_AE_NONE;

	*fetchedCount = 1;

	return true;
}

bool UInputConnection::setSelection(unsigned long count, const TS_SELECTION_ACP *selections)
{
	if (count != 1)
		return false;

	int selStart = selections[0].acpStart;
	int selEnd = selections[0].acpEnd;

	if (selStart == selEnd)
		mTextView->getEditable()->setSelectionForceNotify(selStart);
	else
		mTextView->getEditable()->setSelectionForceNotify(selStart, selEnd);

	return true;
}

std::wstring UInputConnection::getText(long start, long end, long maxLength)
{
	std::wstring totalText = mTextView->getText();

	if (end == -1)
	{
		if (static_cast<unsigned long>(start + maxLength) <= totalText.length())
			return totalText.substr(start, maxLength);
		else
			return totalText.substr(start, totalText.length() - start);
	}
	else
	{
		if (end - start >= maxLength)
			return totalText.substr(start, maxLength);
		else
			return totalText.substr(start, end - start);
	}
}

void UInputConnection::setText(long start, long end, std::wstring newText)
{
	if (start == end)
		mTextView->getEditable()->insert(newText, start);
	else
		mTextView->getEditable()->replace(newText, start, end - start);
}

long UInputConnection::getTextLength()
{
	return mTextView->getText().length();
}

bool UInputConnection::getTextPositionAtPoint(const POINT *pt, DWORD dwFlags, long *pacp)
{
	return true;
}

bool UInputConnection::getTextBound(long start, long end, RECT *prc, BOOL *pfClipped)
{
	URect bound = mTextView->getBoundInScreen();
	URectF textBound = mTextView->getSelectionBound(start, end);

	prc->left = static_cast<long>(
		bound.left + textBound.left + mTextView->getPaddingLeft() - mTextView->getScrollX());
	prc->top = static_cast<long>(
		bound.top + textBound.top + mTextView->getPaddingTop() - mTextView->getScrollY());
	prc->right = static_cast<long>(
		bound.left + textBound.right + mTextView->getPaddingLeft() - mTextView->getScrollX());
	prc->bottom = static_cast<long>(
		bound.top + textBound.bottom + mTextView->getPaddingTop() - mTextView->getScrollY());

	return true;
}

void UInputConnection::getTextViewBound(RECT *prc)
{
	URect bound = mTextView->getBoundInScreen();
	prc->left = bound.left;
	prc->top = bound.top;
	prc->right = bound.right;
	prc->bottom = bound.bottom;
}

void UInputConnection::insertTextAtSelection(
	DWORD dwFlags, std::wstring text,
	LONG *pacpStart, LONG *pacpEnd, TS_TEXTCHANGE *pChange)
{
	switch (dwFlags)
	{
	case 0:
		*pacpStart = mTextView->getSelectionStart();
		*pacpEnd = mTextView->getSelectionEnd();
		break;

	case TF_IAS_NOQUERY:
		pacpStart = 0;
		pacpEnd = 0;
		break;

	case TF_IAS_QUERYONLY:
		*pacpStart = mTextView->getSelectionStart();
		*pacpEnd = mTextView->getSelectionEnd();
		break;
	}
}

HWND UInputConnection::getWindowHandle()
{
	return mTextView->getWindow()->getWindowHandle();
}