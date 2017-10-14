#pragma once
#include "View.h"

class TextView2 : public View
{
private:

	void initTextView();

	HRESULT createFontFace(PCWCHAR fontName, IDWriteFontFace **fontFace);

public:
	TextView2(UWindow *wnd);
	TextView2(UWindow *wnd, int id);
	virtual ~TextView2();
};

