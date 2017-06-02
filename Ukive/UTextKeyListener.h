#pragma once

class UEditable;

class UTextKeyListener
{
private:
	std::wstring numpad(int keyCode);
	std::wstring number(int keyCode);
	std::wstring alphabet(int keyCode, bool upper);
	std::wstring symbol(int keyCode);

	bool metaKey(int keyCode, bool down);

	void backspace(UEditable *editable);
	void newline(UEditable *editable);

	bool isNumpad(int keyCode);
	bool isNumber(int keyCode);
	bool isAlphabet(int keyCode);
	
public:
	UTextKeyListener();
	~UTextKeyListener();

	void onKeyDown(UEditable *editable, bool canEdit, bool canSelect, int keyCode);
	void onKeyUp(UEditable *editable, bool canEdit, bool canSelect, int keyCode);
};