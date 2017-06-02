#include "UCommon.h"
#include "WICManager.h"
#include "UBitmapFactory.h"


UBitmapFactory::UBitmapFactory(
	WICManager *wicMgr, 
	UComPtr<ID2D1DeviceContext> d2DDeviceContext)
{
	if (wicMgr == nullptr || d2DDeviceContext.get() == nullptr)
		throw std::invalid_argument("UBitmapFactory-Constructor(): null params.");

	mWICManager = wicMgr;
	mD2DDeviceContext = d2DDeviceContext;
}


UBitmapFactory::~UBitmapFactory()
{
}


UComPtr<ID2D1Bitmap> UBitmapFactory::decodeFile(std::wstring fileName)
{
	HRESULT hr;
	UComPtr<IWICBitmapSource> source;
	UComPtr<IWICBitmapSource> destFormatSource;

	hr = mWICManager->decodeFile(fileName, &source);
	if (SUCCEEDED(hr))
	{
		hr = mWICManager->convertForD2D(source.get(), &destFormatSource);
	}

	UComPtr<ID2D1Bitmap1> d2dBitmap;

	if (SUCCEEDED(hr))
	{
		hr = mD2DDeviceContext->CreateBitmapFromWicBitmap(
			destFormatSource.get(), 0, &d2dBitmap);
	}

	if (SUCCEEDED(hr))
		return d2dBitmap.cast<ID2D1Bitmap>();

	return nullptr;
}