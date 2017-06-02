#include "UCommon.h"
#include "USpan.h"
#include "InteractableSpan.h"
#include "UEditable.h"


UEditable::UEditable(std::wstring text)
{
	mText = text;
	mSelectionStart = 0;
	mSelectionEnd = 0;
}


UEditable::~UEditable()
{
}


//The possible text changes include insert, delete, and replace. 
//For example, if you replace the first "t" of "text" with "T", acpStart =0, acpOldEnd =1, and acpNewEnd =1. 
//If you delete the last "t", acpStart =3, acpOldEnd =4, and acpNewEnd =3. 
//If an "a" is inserted between "e" and "x", acpStart =2, acpOldEnd =2, and acpNewEnd =3.
void UEditable::notifyTextChanged(
	uint32_t start, uint32_t oldEnd, uint32_t newEnd)
{
	for (auto it = mSpanList.begin();
		it != mSpanList.end(); ++it)
	{
		uint32_t spanStart = (*it)->getStart();
		uint32_t spanEnd = (*it)->getEnd();

		if (spanStart >= start && spanStart < oldEnd)
		{
		}
	}

	for (auto it = mWatcherList.begin();
		it != mWatcherList.end(); ++it)
	{
		(*it)->onTextChanged(this, start, oldEnd, newEnd);
	}
}

void UEditable::notifySelectionChanged(
	uint32_t ns, uint32_t ne, 
	uint32_t os, uint32_t oe)
{
	for (auto it = mWatcherList.begin();
		it != mWatcherList.end(); ++it)
	{
		(*it)->onSelectionChanged(ns, ne, os, oe);
	}
}

void UEditable::notifyEditWatcher(
	int start, int oldEnd, int newEnd,
	uint32_t ns, uint32_t ne, 
	uint32_t os, uint32_t oe)
{
	for (auto it = mWatcherList.begin();
		it != mWatcherList.end(); ++it)
	{
		(*it)->onTextChanged(this, start, oldEnd, newEnd);
		(*it)->onSelectionChanged(ns, ne, os, oe);
	}
}

void UEditable::notifySpanChanged(
	USpan *span, UEditWatcher::SpanChange action)
{
	for (auto it = mWatcherList.begin();
		it != mWatcherList.end(); ++it)
	{
		(*it)->onSpanChanged(span, action);
	}
}


uint32_t UEditable::length()
{
	return mText.length();
}


void UEditable::append(std::wstring text)
{
	this->insert(text, mText.length());
}

void UEditable::insert(std::wstring text, uint32_t position)
{
	if (!text.empty())
	{
		mText.insert(position, text);
		this->notifyTextChanged(position, position, position + text.length());
	}
}

void UEditable::remove(uint32_t start, uint32_t length)
{
	if (!mText.empty())
		this->replace(L"", start, length);
}

void UEditable::replace(std::wstring text, uint32_t start, uint32_t length)
{
	mText.replace(start, length, text);
	notifyTextChanged(start, start + 1, start + text.length() - length + 1);
}

void UEditable::clear()
{
	if (!mText.empty())
	{
		uint32_t oldStart = mSelectionStart;
		uint32_t oldEnd = mSelectionEnd;
		uint32_t oldLength = mText.length();

		mText.clear();

		mSelectionStart = 0;
		mSelectionEnd = 0;

		if (oldStart != 0 || oldEnd != 0)
			notifyEditWatcher(
				0, oldLength, 0,
				mSelectionStart, mSelectionEnd, 
				oldStart, oldEnd);
		else
			notifyTextChanged(0, oldLength, 0);
	}
}


void UEditable::replace(std::wstring find, std::wstring replacement)
{
	size_t first = mText.find(find);
	if (first != std::wstring::npos)
	{
		mText.replace(first, find.length(), replacement);
		notifyTextChanged(first, first + 1, first + replacement.length() - find.length() + 1);
	}
}

void UEditable::replaceAll(std::wstring find, std::wstring replacement)
{
	size_t first = mText.find(find);
	while (first != std::wstring::npos)
	{
		mText.replace(first, find.length(), replacement);
		first = mText.find(find, first + replacement.length());
	}
}


void UEditable::insert(std::wstring text)
{
	uint32_t oldStart = mSelectionStart;
	uint32_t oldEnd = mSelectionEnd;

	if (oldStart == oldEnd)
	{
		mText.insert(oldEnd, text);
		mSelectionStart += text.length();
		mSelectionEnd += text.length();

		this->notifyEditWatcher(
			mSelectionStart, mSelectionStart, mSelectionStart + text.length(),
			mSelectionStart, mSelectionEnd, oldStart, oldEnd);
	}
}

void UEditable::remove()
{
	this->replace(L"");
}

void UEditable::replace(std::wstring text)
{
	uint32_t oldStart = mSelectionStart;
	uint32_t oldEnd = mSelectionEnd;

	bool hasSelection = oldStart != oldEnd;

	if (hasSelection)
	{
		mText.replace(oldStart, oldEnd - oldStart, text);
		mSelectionStart = mSelectionEnd = oldStart + text.length();

		this->notifyEditWatcher(
			oldStart, oldStart + 1, oldStart + text.length() - (oldEnd - oldStart) + 1,
			mSelectionStart, mSelectionEnd, oldStart, oldEnd);
	}
	else
		return;
}


void UEditable::setSelection(uint32_t selection)
{
	this->setSelection(selection, selection);
}

void UEditable::setSelection(uint32_t start, uint32_t end)
{
	if (start == mSelectionStart && end == mSelectionEnd)
		return;

	this->setSelectionForceNotify(start, end);
}

void UEditable::setSelectionForceNotify(uint32_t selection)
{
	this->setSelectionForceNotify(selection, selection);
}

void UEditable::setSelectionForceNotify(uint32_t start, uint32_t end)
{
	if (start < 0) start = 0;
	if (start > mText.length()) start = mText.length();

	if (end < 0) end = 0;
	if (end > mText.length()) end = mText.length();

	if (start > end)
	{
		uint32_t tmp = start;
		start = end;
		end = tmp;
	}

	int oldSeStart = mSelectionStart;
	int oldSeEnd = mSelectionEnd;

	mSelectionStart = start;
	mSelectionEnd = end;

	notifySelectionChanged(start, end, oldSeStart, oldSeEnd);
}


uint32_t UEditable::getSelectionStart()
{
	return mSelectionStart;
}

uint32_t UEditable::getSelectionEnd()
{
	return mSelectionEnd;
}

bool UEditable::hasSelection()
{
	return mSelectionStart != mSelectionEnd;
}

std::wstring UEditable::getSelection()
{
	if (mSelectionStart == mSelectionEnd)
		return L"";

	return mText.substr(mSelectionStart, mSelectionEnd - mSelectionStart);
}


wchar_t UEditable::at(size_t pos)
{
	return mText.at(pos);
}

std::wstring UEditable::toString()
{
	return mText;
}


void UEditable::addSpan(USpan *span)
{
	mSpanList.push_back(std::shared_ptr<USpan>(span));

	this->notifySpanChanged(
		span, UEditWatcher::SpanChange::ADD);
}

void UEditable::removeSpan(std::size_t index)
{
	USpan *span = getSpan(index);
	if (span)
	{
		this->notifySpanChanged(
			span, UEditWatcher::SpanChange::REMOVE);

		mSpanList.erase(mSpanList.begin() + index);
	}
}

void UEditable::removeAllSpan()
{
	while (!mSpanList.empty())
		removeSpan(0);
}

USpan *UEditable::getSpan(std::size_t index)
{
	return mSpanList.at(index).get();
}

std::size_t UEditable::getSpanCount()
{
	return mSpanList.size();
}


void UEditable::addEditWatcher(UEditWatcher *watcher)
{
	mWatcherList.push_back(watcher);
}

void UEditable::removeEditWatcher(UEditWatcher *watcher)
{
	for (auto it = mWatcherList.begin();
		it != mWatcherList.end();)
	{
		if (watcher == (*it))
			it = mWatcherList.erase(it);
		else
			++it;
	}
}