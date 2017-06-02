#pragma once

class ClipboardManager
{
public:
	ClipboardManager();
	~ClipboardManager();

	static void saveToClipboard(std::wstring text);
	static std::wstring getFromClipboard();
};