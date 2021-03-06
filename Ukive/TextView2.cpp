#include "UCommon.h"
#include "UWindow.h"
#include "URenderer.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "TextView2.h"


TextView2::TextView2(UWindow *wnd)
	:View(wnd)
{
	initTextView();
}

TextView2::TextView2(UWindow *wnd, int id)
	:View(wnd, id)
{
	initTextView();
}

TextView2::~TextView2()
{
}


void TextView2::initTextView()
{
	IDWriteFontFace *fontFace = nullptr;
	HRESULT hr = createFontFace(L"΢���ź�", &fontFace);

	auto dwriteFactory = getWindow()->getApplication()->getDeviceManager()
		->getDWriteFactory();

	IDWriteTextAnalyzer *textAnalyzer = nullptr;
	hr = dwriteFactory->CreateTextAnalyzer(&textAnalyzer);
	if (SUCCEEDED(hr))
	{
	}
}

HRESULT TextView2::createFontFace(PCWCHAR fontName, IDWriteFontFace **fontFace)
{
	auto dwriteFactory = getWindow()->getApplication()->getDeviceManager()
		->getDWriteFactory();

	IDWriteFontCollection *fontCollection = nullptr;
	HRESULT hr = dwriteFactory->GetSystemFontCollection(&fontCollection);
	if (SUCCEEDED(hr))
	{
		BOOL exist;
		UINT32 index;
		hr = fontCollection->FindFamilyName(fontName, &index, &exist);
		if (SUCCEEDED(hr))
		{
			if (exist == TRUE)
			{
				IDWriteFontFamily *fontFamily = nullptr;
				hr = fontCollection->GetFontFamily(index, &fontFamily);
				if (SUCCEEDED(hr))
				{
					IDWriteFont *font = nullptr;
					hr = fontFamily->GetFirstMatchingFont(
						DWRITE_FONT_WEIGHT_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						DWRITE_FONT_STYLE_NORMAL,
						&font);
					if (SUCCEEDED(hr))
					{
						IDWriteFontFace *_fontFace = nullptr;
						hr = font->CreateFontFace(&_fontFace);
						if (SUCCEEDED(hr))
						{
							*fontFace = _fontFace;
						}

						font->Release();
					}

					fontFamily->Release();
				}
			}
		}

		fontCollection->Release();
	}

	return hr;
}