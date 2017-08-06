#include "UCommon.h"
#include "UWindow.h"
#include "URenderer.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "UTextView2.h"


UTextView2::UTextView2(UWindow *wnd)
	:UWidget(wnd)
{
	initTextView();
}

UTextView2::UTextView2(UWindow *wnd, int id)
	:UWidget(wnd, id)
{
	initTextView();
}

UTextView2::~UTextView2()
{
}


void UTextView2::initTextView()
{
	IDWriteFontFace *fontFace = nullptr;
	HRESULT hr = createFontFace(L"Î¢ÈíÑÅºÚ", &fontFace);

	auto dwriteFactory = getWindow()->getApplication()->getDeviceManager()
		->getDWriteFactory();

	IDWriteTextAnalyzer *textAnalyzer = nullptr;
	hr = dwriteFactory->CreateTextAnalyzer(&textAnalyzer);
	if (SUCCEEDED(hr))
	{
	}
}

HRESULT UTextView2::createFontFace(PCWCHAR fontName, IDWriteFontFace **fontFace)
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