#pragma once

class WICManager;

class UBitmapFactory
{
private:
	WICManager *mWICManager;
	UComPtr<ID2D1DeviceContext> mD2DDeviceContext;

public:
	UBitmapFactory(
		WICManager *wicMgr, 
		UComPtr<ID2D1DeviceContext> d2DDeviceContext);
	~UBitmapFactory();

	UComPtr<ID2D1Bitmap> decodeFile(std::wstring fileName);
};