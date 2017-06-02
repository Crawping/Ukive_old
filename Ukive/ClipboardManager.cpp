#include "UCommon.h"
#include "ClipboardManager.h"


ClipboardManager::ClipboardManager()
{
}

ClipboardManager::~ClipboardManager()
{
}


void ClipboardManager::saveToClipboard(std::wstring text)
{
	if (::OpenClipboard(nullptr))
	{
		::EmptyClipboard();

		std::size_t space = (text.length() + 1) * sizeof(wchar_t);

		HANDLE hHandle = ::GlobalAlloc(GMEM_FIXED, space);
		wchar_t* pData = (wchar_t*)::GlobalLock(hHandle);

		text._Copy_s(pData, text.length(), text.length());
		pData[text.length()] = L'\0';

		::SetClipboardData(CF_UNICODETEXT, hHandle);
		::GlobalUnlock(hHandle);
		::CloseClipboard();
	}
}

std::wstring ClipboardManager::getFromClipboard()
{
	std::wstring content = L"";

	if (::OpenClipboard(nullptr))
	{
		HGLOBAL hMem = ::GetClipboardData(CF_UNICODETEXT);
		if (hMem != nullptr)
		{
			wchar_t* lpStr = (wchar_t*)::GlobalLock(hMem);
			if (lpStr != nullptr)
			{
				content = std::wstring(lpStr);
				::GlobalUnlock(hMem);
			}
		}

		::CloseClipboard();
	}

	return content;
}