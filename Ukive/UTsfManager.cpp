﻿#include "UCommon.h"
#include "UTsfEditor.h"
#include "TextView.h"
#include "UTsfManager.h"

UTsfManager::UTsfManager()
{
	mSink = nullptr;

	mAlpnSinkCookie = TF_INVALID_COOKIE;
	mOpenModeSinkCookie = TF_INVALID_COOKIE;
	mConvModeSinkCookie = TF_INVALID_COOKIE;
}

UTsfManager::~UTsfManager()
{
}

HRESULT UTsfManager::init()
{
	HRESULT hr = E_FAIL;

	hr = CoCreateInstance(CLSID_TF_ThreadMgr,
		nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(ITfThreadMgr),
		(void**)&mThreadMgr);
	if (FAILED(hr))
		return hr;

	hr = mThreadMgr->Activate(&mClientId);
	if (FAILED(hr))
		return false;

	return S_OK;
}

void UTsfManager::close()
{
	mThreadMgr->Deactivate();
}

TfClientId UTsfManager::getClientId()
{
	return mClientId;
}

UComPtr<ITfThreadMgr> UTsfManager::getThreadManager()
{
	return mThreadMgr;
}





HRESULT UTsfManager::setupCompartmentSinks(ITfCompartment *openMode, ITfCompartment *convMode)
{
	HRESULT hr = E_FAIL;
	ITfSource *srcOpenMode = 0;
	ITfSource *srcConvMode = 0;

	if (openMode == 0 || convMode == 0)
		return E_FAIL;

	if (SUCCEEDED(hr = openMode->QueryInterface(IID_ITfSource, (void**)&srcOpenMode)))
	{
		if (mOpenModeSinkCookie != TF_INVALID_COOKIE)
		{
			srcOpenMode->UnadviseSink(mOpenModeSinkCookie);
			mOpenModeSinkCookie = TF_INVALID_COOKIE;
		}

		if (SUCCEEDED(hr = srcOpenMode->AdviseSink(IID_ITfCompartmentEventSink, (ITfCompartmentEventSink*)mSink, &mOpenModeSinkCookie)))
		{
			if (SUCCEEDED(hr = convMode->QueryInterface(IID_ITfSource, (void**)&srcConvMode)))
			{
				if (mConvModeSinkCookie != TF_INVALID_COOKIE)
				{
					srcConvMode->UnadviseSink(mConvModeSinkCookie);
					mConvModeSinkCookie = TF_INVALID_COOKIE;
				}

				hr = srcConvMode->AdviseSink(IID_ITfCompartmentEventSink, (ITfCompartmentEventSink*)mSink, &mConvModeSinkCookie);
			}
			srcConvMode->Release();
		}
		srcOpenMode->Release();
	}

	return hr;
}

HRESULT UTsfManager::releaseCompartmentSinks()
{
	HRESULT hr = E_FAIL;

	ITfCompartmentMgr *cm = 0;
	ITfCompartment *openMode = 0;
	ITfCompartment *convMode = 0;

	hr = getCompartments(&cm, &openMode, &convMode);

	ITfSource *srcOpenMode = 0;
	if (SUCCEEDED(hr))
	{
		hr = openMode->QueryInterface(IID_ITfSource, (void**)&srcOpenMode);

		if (mOpenModeSinkCookie != TF_INVALID_COOKIE)
		{
			srcOpenMode->UnadviseSink(mOpenModeSinkCookie);
			mOpenModeSinkCookie = TF_INVALID_COOKIE;
		}
	}

	ITfSource *srcConvMode = 0;
	if (SUCCEEDED(hr))
	{
		hr = convMode->QueryInterface(IID_ITfSource, (void**)&srcConvMode);

		if (mConvModeSinkCookie != TF_INVALID_COOKIE)
		{
			srcConvMode->UnadviseSink(mConvModeSinkCookie);
			mConvModeSinkCookie = TF_INVALID_COOKIE;
		}
	}

	return hr;
}

HRESULT UTsfManager::getCompartments(ITfCompartmentMgr **cm, ITfCompartment **openMode, ITfCompartment **convMode)
{
	HRESULT hr = E_FAIL;
	ITfCompartmentMgr *_cm = 0;
	ITfCompartment *_openMode = 0;
	ITfCompartment *_convMode = 0;

	if (mThreadMgr.get() == 0)
		return E_FAIL;

	hr = mThreadMgr->QueryInterface(IID_ITfCompartmentMgr, (void**)&_cm);
	if (FAILED(hr))
		return hr;

	hr = _cm->GetCompartment(GUID_COMPARTMENT_KEYBOARD_OPENCLOSE, &_openMode);
	if (FAILED(hr))
		return hr;

	hr = _cm->GetCompartment(GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION, &_convMode);
	if (FAILED(hr))
		return hr;

	*cm = _cm;
	*openMode = _openMode;
	*convMode = _convMode;

	return S_OK;
}

HRESULT UTsfManager::updateImeState()
{
	ITfCompartmentMgr *cm=0;
	ITfCompartment *openMode = 0;
	ITfCompartment *convMode = 0;

	if (SUCCEEDED(getCompartments(&cm, &openMode, &convMode)))
	{
		VARIANT valOpenMode;
		VARIANT valConvMode;
		VariantInit(&valOpenMode);
		VariantInit(&valConvMode);

		HRESULT hr = openMode->GetValue(&valOpenMode);
		hr = convMode->GetValue(&valConvMode);

		valOpenMode.vt = VT_I4;
		valConvMode.vt = VT_I4;

		valOpenMode.lVal = TRUE;
		valConvMode.lVal = 0;

		hr = openMode->SetValue(mClientId, &valOpenMode);
		hr = convMode->SetValue(mClientId, &valConvMode);

		VariantClear(&valOpenMode);
		VariantClear(&valConvMode);

		setupCompartmentSinks(openMode, convMode);

		openMode->Release();
		convMode->Release();
		cm->Release();
	}

	return S_OK;
}


HRESULT UTsfManager::setupSinks()
{
	HRESULT hr = E_FAIL;

	UComPtr<ITfSource> src;
	UComPtr<ITfCompartmentMgr> cm;
	UComPtr<ITfCompartment> openMode;
	UComPtr<ITfCompartment> convMode;

	mSink = new UTsfSink(this);

	hr = mThreadMgr->QueryInterface(__uuidof(ITfSource), (void **)&src);

	if (SUCCEEDED(hr))
		hr = src->AdviseSink(__uuidof(ITfInputProcessorProfileActivationSink), (ITfInputProcessorProfileActivationSink*)mSink, &mAlpnSinkCookie);

	if (SUCCEEDED(hr))
	{
		hr = getCompartments(&cm, &openMode, &convMode);

		VARIANT valOpenMode;
		VARIANT valConvMode;
		VariantInit(&valOpenMode);
		VariantInit(&valConvMode);

		hr = openMode->GetValue(&valOpenMode);
		hr = convMode->GetValue(&valConvMode);

		valOpenMode.vt = VT_I4;
		valConvMode.vt = VT_I4;

		valOpenMode.lVal = TRUE;
		valConvMode.lVal = 0;

		hr = openMode->SetValue(mClientId, &valOpenMode);
		hr = convMode->SetValue(mClientId, &valConvMode);

		VariantClear(&valOpenMode);
		VariantClear(&valConvMode);

		hr = setupCompartmentSinks(openMode.get(), convMode.get());
	}

	return hr;
}

HRESULT UTsfManager::releaseSinks()
{
	HRESULT hr=E_FAIL;
	UComPtr<ITfSource> source;

	if (mThreadMgr.get() && SUCCEEDED(mThreadMgr->QueryInterface(__uuidof(ITfSource), (void **)&source)))
	{
		hr = source->UnadviseSink(mAlpnSinkCookie);
		hr = releaseCompartmentSinks();

		mAlpnSinkCookie = TF_INVALID_COOKIE;
	}

	return hr;
}


UTsfSink::UTsfSink(UTsfManager *tsfMgr)
{
	mRefCount = 1;
	mTsfMgr = tsfMgr;
}

UTsfSink::~UTsfSink()
{

}

STDAPI UTsfSink::QueryInterface(REFIID riid, void **ppvObj)
{
	if (ppvObj == NULL)
		return E_INVALIDARG;

	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObj = reinterpret_cast<IUnknown *>(this);
	}
	else if (IsEqualIID(riid, __uuidof(ITfInputProcessorProfileActivationSink)))
	{
		*ppvObj = (ITfInputProcessorProfileActivationSink*)this;
	}
	else if (IsEqualIID(riid, __uuidof(ITfCompartmentEventSink)))
	{
		*ppvObj = (ITfCompartmentEventSink*)this;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDAPI_(ULONG) UTsfSink::AddRef(void)
{
	return InterlockedIncrement(&mRefCount);
}

STDAPI_(ULONG) UTsfSink::Release(void)
{
	LONG cr = InterlockedDecrement(&mRefCount);

	if (mRefCount == 0)
		delete this;

	return cr;
}


//切换输入法时响应
STDAPI UTsfSink::OnActivated(DWORD dwProfileType, LANGID langid, REFCLSID clsid, REFGUID catid, REFGUID guidProfile, HKL hkl, DWORD dwFlags)
{
	switch (dwProfileType)
	{
	case TF_PROFILETYPE_INPUTPROCESSOR:
		break;

	case TF_PROFILETYPE_KEYBOARDLAYOUT:
		break;
	}

	switch (dwFlags)
	{
	case 2:
		break;

	case 3:
		//mTsfMgr->updateImeState();
		break;
	}

	return S_OK;
}

//同一输入法中-英切换，日-英切换等等
STDAPI UTsfSink::OnChange(REFGUID rguid)
{
	return S_OK;
}