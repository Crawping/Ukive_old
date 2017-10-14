#pragma once

class USpan;
class View;
class UInputEvent;

class UEditable
{
public:
	struct UEditWatcher
	{
		enum SpanChange
		{
			ADD,
			REMOVE,
			EDIT,
		};

		virtual void onTextChanged(
			UEditable *editable, 
			int start, int oldEnd, int newEnd) = 0;

		virtual void onSelectionChanged(
			uint32_t ns, uint32_t ne, 
			uint32_t os, uint32_t oe) = 0;

		virtual void onSpanChanged(
			USpan *span, SpanChange action) = 0;
	};

private:
	std::wstring mText;
	std::list<UEditWatcher*> mWatcherList;
	std::vector<std::shared_ptr<USpan>> mSpanList;

	uint32_t mSelectionStart;
	uint32_t mSelectionEnd;

	void notifyTextChanged(
		uint32_t start, uint32_t oldEnd, uint32_t newEnd);
	void notifySelectionChanged(
		uint32_t ns, uint32_t ne, uint32_t os, uint32_t oe);
	void notifyEditWatcher(
		int start, int oldEnd, int newEnd,
		uint32_t ns, uint32_t ne, uint32_t os, uint32_t oe);
	void notifySpanChanged(
		USpan *span, UEditWatcher::SpanChange action);

public:
	UEditable(std::wstring text);
	~UEditable();

	uint32_t length();

	void append(std::wstring text);
	void insert(std::wstring text, uint32_t position);
	void remove(uint32_t start, uint32_t length);
	void replace(std::wstring text, uint32_t start, uint32_t length);
	void clear();

	void replace(std::wstring find, std::wstring replacement);
	void replaceAll(std::wstring find, std::wstring replacement);

	void insert(std::wstring text);
	void remove();
	void replace(std::wstring text);

	void setSelection(uint32_t selection);
	void setSelection(uint32_t start, uint32_t end);

	void setSelectionForceNotify(uint32_t selection);
	void setSelectionForceNotify(uint32_t start, uint32_t end);

	uint32_t getSelectionStart();
	uint32_t getSelectionEnd();
	bool hasSelection();
	std::wstring getSelection();

	wchar_t at(size_t pos);
	std::wstring toString();

	void addSpan(USpan *span);
	void removeSpan(std::size_t index);
	void removeAllSpan();
	USpan *getSpan(std::size_t index);
	std::size_t getSpanCount();

	void addEditWatcher(UEditWatcher *watcher);
	void removeEditWatcher(UEditWatcher *watcher);
};