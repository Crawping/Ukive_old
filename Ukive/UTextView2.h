#pragma once
#include "UWidget.h"

class UTextView2 : public UWidget
{
private:

	void initTextView();

	HRESULT createFontFace(PCWCHAR fontName, IDWriteFontFace **fontFace);

public:
	UTextView2(UWindow *wnd);
	UTextView2(UWindow *wnd, int id);
	virtual ~UTextView2();
};

