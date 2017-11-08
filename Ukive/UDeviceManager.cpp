#include "UCommon.h"
#include "UDeviceManager.h"

UComPtr<IDXGIFactory2> UDeviceManager::sDXGIFactory;
UComPtr<ID2D1Factory1> UDeviceManager::sD2DFactory;
UComPtr<IDWriteFactory1> UDeviceManager::sDWriteFactory;

UComPtr<ID2D1SolidColorBrush> UDeviceManager::sSolidBrush;
UComPtr<ID2D1BitmapBrush> UDeviceManager::sBitmapBrush;

UComPtr<ID2D1SolidColorBrush> UDeviceManager::sCanvasSolidBrush;
UComPtr<ID2D1BitmapBrush> UDeviceManager::sCanvasBitmapBrush;

UDeviceManager::UDeviceManager()
{
}


UDeviceManager::~UDeviceManager()
{
}


HRESULT UDeviceManager::init()
{
	RH(createPersistance());
	RH(createD3DDevice());
	RH(createD2DDevice());

	return S_OK;
}

void UDeviceManager::close()
{
	releaseD2DDevice();
	releaseD3DDevice();
	releasePersistance();
}


UComPtr<ID2D1DeviceContext> UDeviceManager::createD2DDeviceContext()
{
	UComPtr<ID2D1DeviceContext> dc;

	mD2DDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&dc);

	return dc;
}


HRESULT UDeviceManager::createPersistance()
{
	RH(CreateDXGIFactory1(
		__uuidof(IDXGIFactory2), 
		(void**)(&sDXGIFactory)));
	RH(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		&sD2DFactory));
	RH(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(IDWriteFactory1), 
		reinterpret_cast<IUnknown**>(&sDWriteFactory)));

	UINT adapterIndex = 0;
	HRESULT adapterResult = S_OK;
	UComPtr<IDXGIAdapter1> adapter;

	while (SUCCEEDED(adapterResult))
	{
		adapterResult = sDXGIFactory->EnumAdapters1(adapterIndex, &adapter);
		if (adapter.get())
		{
			UINT outputIndex = 0;
			HRESULT outputResult = S_OK;
			UComPtr<IDXGIOutput> output;

			while (SUCCEEDED(outputResult))
			{
				outputResult = adapter->EnumOutputs(outputIndex, &output);
				if (output.get())
				{
					mCurAdapter = adapter;
					mCurOutput = output;

					++outputIndex;
					output.reset();
				}
			}

			++adapterIndex;
			adapter.reset();
		}
	}

	return S_OK;
}

void UDeviceManager::releasePersistance()
{
	sDXGIFactory.reset();
	sD2DFactory.reset();
	sDWriteFactory.reset();
}


HRESULT UDeviceManager::createD3DDevice()
{
	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	RH(D3D11CreateDevice(0,
		D3D_DRIVER_TYPE_HARDWARE,
		0, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		featureLevel, 4, D3D11_SDK_VERSION,
		&mD3DDevice, 0, &mD3DDeviceContext));

	RH(mD3DDevice->QueryInterface(
		__uuidof(IDXGIDevice2), (void **)&mDXGIDevice));

	return S_OK;
}

void UDeviceManager::releaseD3DDevice()
{
	mD3DDeviceContext.reset();
	mD3DDevice.reset();
	mDXGIDevice.reset();
}


HRESULT UDeviceManager::createD2DDevice()
{
	RH(sD2DFactory->CreateDevice(
		mDXGIDevice.get(), &mD2DDevice));

	RH(mD2DDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&mD2DDeviceContext));

	RH(mD2DDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &sSolidBrush));
	RH(mD2DDeviceContext->CreateBitmapBrush(nullptr, &sBitmapBrush));

	RH(mD2DDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &sCanvasSolidBrush));
	RH(mD2DDeviceContext->CreateBitmapBrush(nullptr, &sCanvasBitmapBrush));

	return S_OK;
}

void UDeviceManager::releaseD2DDevice()
{
	sSolidBrush.reset();
	sBitmapBrush.reset();
	sCanvasSolidBrush.reset();
	sCanvasBitmapBrush.reset();

	mD2DDeviceContext.reset();
	mD2DDevice.reset();
}


UComPtr<IDXGIAdapter1> UDeviceManager::getCurAdapter()
{
	return mCurAdapter;
}

UComPtr<IDXGIOutput> UDeviceManager::getCurOutput()
{
	return mCurOutput;
}


UComPtr<IDXGIFactory2> UDeviceManager::getDXGIFactory() { return sDXGIFactory; }

UComPtr<ID2D1Factory1> UDeviceManager::getD2DFactory() { return sD2DFactory; }

UComPtr<IDWriteFactory1> UDeviceManager::getDWriteFactory() { return sDWriteFactory; }

UComPtr<IDXGIDevice2> UDeviceManager::getDXGIDevice() { return mDXGIDevice; }

UComPtr<ID2D1Device> UDeviceManager::getD2DDevice() { return mD2DDevice; }

UComPtr<ID3D11Device> UDeviceManager::getD3DDevice() { return mD3DDevice; }

UComPtr<ID3D11DeviceContext> UDeviceManager::getD3DDeviceContext() { return mD3DDeviceContext; }


float UDeviceManager::dip(float value)
{
	float dpiX;
	float dpiY;
	sD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
	return (dpiX / 96.f)*value;
}