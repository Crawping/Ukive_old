#pragma once
#include "UComPtr.h"

class UDeviceManager
{
public:
	static UComPtr<IDXGIFactory2> sDXGIFactory;      //Win8 and 7-PU.
	static UComPtr<ID2D1Factory1> sD2DFactory;       //Win8 and 7-PU.
	static UComPtr<IDWriteFactory1> sDWriteFactory;  //Win8 and 7-PU.

	static UComPtr<ID2D1SolidColorBrush> sSolidBrush;
	static UComPtr<ID2D1BitmapBrush> sBitmapBrush;

	static UComPtr<ID2D1SolidColorBrush> sCanvasSolidBrush;
	static UComPtr<ID2D1BitmapBrush> sCanvasBitmapBrush;

private:
	UComPtr<IDXGIDevice2> mDXGIDevice;             //Win8 and 7-PU.
	UComPtr<ID2D1Device> mD2DDevice;               //Win8 and 7-PU.
	UComPtr<ID2D1DeviceContext> mD2DDeviceContext; //Win8 and 7-PU.

	UComPtr<ID3D11Device> mD3DDevice;                //Win7.
	UComPtr<ID3D11DeviceContext> mD3DDeviceContext;  //Win7.

	UComPtr<IDXGIAdapter1> mCurAdapter;
	UComPtr<IDXGIOutput> mCurOutput;

	HRESULT createPersistance();
	void releasePersistance();

	HRESULT createD3DDevice();
	void releaseD3DDevice();

	HRESULT createD2DDevice();
	void releaseD2DDevice();

public:
	UDeviceManager();
	~UDeviceManager();

	HRESULT init();
	void close();

	UComPtr<IDXGIAdapter1> getCurAdapter();
	UComPtr<IDXGIOutput> getCurOutput();

	UComPtr<IDXGIFactory2> getDXGIFactory();
	UComPtr<ID2D1Factory1> getD2DFactory();
	UComPtr<IDWriteFactory1> getDWriteFactory();

	UComPtr<IDXGIDevice2> getDXGIDevice();
	UComPtr<ID2D1Device> getD2DDevice();
	UComPtr<ID2D1DeviceContext> getD2DDeviceContext();

	UComPtr<ID3D11Device> getD3DDevice();
	UComPtr<ID3D11DeviceContext> getD3DDeviceContext();

	static float dip(float value);
};