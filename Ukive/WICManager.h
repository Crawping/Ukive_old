#pragma once
#include "UComPtr.h"

class WICManager
{
private:
	UComPtr<IWICImagingFactory> mWICFactory;

public:
	WICManager();
	~WICManager();

	HRESULT init();
	void close();

	HRESULT convertForD2D(IWICBitmapSource *source, IWICBitmapSource **destination);
	HRESULT decodeFile(std::wstring fileName, IWICBitmapSource **outSource);
	HRESULT getBitmapFromSource(IWICBitmapSource *source, IWICBitmap **outBitmap);
};