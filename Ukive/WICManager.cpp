#include "UCommon.h"
#include "WICManager.h"


WICManager::WICManager()
{
	mWICFactory = 0;
}


WICManager::~WICManager()
{
}


HRESULT WICManager::init()
{
	HRESULT hr;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		0,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&mWICFactory)
		);
	if (FAILED(hr))
		return hr;

	return hr;
}


void WICManager::close()
{
}


HRESULT WICManager::convertForD2D(IWICBitmapSource *source, IWICBitmapSource **destination)
{
	HRESULT hr;
	IWICFormatConverter *converter = 0;

	// Format convert the frame to 32bppPBGRA
	hr = mWICFactory->CreateFormatConverter(&converter);

	if (SUCCEEDED(hr))
	{
		hr = converter->Initialize(
			source,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			0,                            // Specify a particular palette 
			0.f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
			);
	}

	if (SUCCEEDED(hr))
		*destination = converter;

	return hr;
}


HRESULT WICManager::decodeFile(std::wstring fileName, IWICBitmapSource **outSource)
{
	HRESULT hr;
	UComPtr<IWICBitmapDecoder> decoder;

	hr = mWICFactory->CreateDecoderFromFilename(
		fileName.c_str(),                      // Image to be decoded
		0,                                     // Do not prefer a particular vendor
		GENERIC_READ,                          // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,        // Cache metadata when needed
		&decoder);                             // Pointer to the decoder

	// Retrieve the first frame of the image from the decoder
	IWICBitmapFrameDecode *pFrame = 0;

	if (SUCCEEDED(hr))
	{
		hr = decoder->GetFrame(0, &pFrame);
	}

	if (SUCCEEDED(hr))
		*outSource = pFrame;

	return hr;
}


HRESULT WICManager::getBitmapFromSource(IWICBitmapSource *source, IWICBitmap **outBitmap)
{
	HRESULT hr;
	IWICBitmap *bitmap = 0;

	hr = mWICFactory->CreateBitmapFromSource(
		source,
		WICBitmapCacheOnDemand,
		&bitmap);

	if (SUCCEEDED(hr))
		*outBitmap = bitmap;

	return hr;
}