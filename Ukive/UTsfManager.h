#pragma once
#include "UComPtr.h"

class UTsfSink;
class UTsfEditor;
class UInputConnection;

class UTsfManager
{
private:
	UTsfSink *mSink;
	TfClientId mClientId;
	UComPtr<ITfThreadMgr> mThreadMgr;
	
	DWORD mAlpnSinkCookie;
	DWORD mOpenModeSinkCookie;
	DWORD mConvModeSinkCookie;

	HRESULT setupCompartmentSinks(ITfCompartment *openMode, ITfCompartment *convMode);
	HRESULT releaseCompartmentSinks();

	HRESULT getCompartments(ITfCompartmentMgr **cm, ITfCompartment **openMode, ITfCompartment **convMode);

public:
	UTsfManager();
	~UTsfManager();

	HRESULT init();
	void close();

	TfClientId getClientId();
	UComPtr<ITfThreadMgr> getThreadManager();

	HRESULT setupSinks();
	HRESULT releaseSinks();

	HRESULT updateImeState();
};

class UTsfSink : public ITfInputProcessorProfileActivationSink, public ITfCompartmentEventSink
{
private:
	LONG mRefCount;
	UTsfManager *mTsfMgr;
	ITfCompositionView *pComposition;

public:
	UTsfSink(UTsfManager *tsfMgr);
	~UTsfSink();

	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj) override;
	STDMETHODIMP_(ULONG) AddRef(void) override;
	STDMETHODIMP_(ULONG) Release(void) override;

	// ITfInputProcessorProfileActivationSink
	// Notification for keyboard input locale change
	STDMETHODIMP OnActivated(
		DWORD dwProfileType, LANGID langid, REFCLSID clsid, REFGUID catid, REFGUID guidProfile, HKL hkl, DWORD dwFlags) override;

	// ITfCompartmentEventSink
	// Notification for open mode (toggle state) change
	STDMETHODIMP OnChange(REFGUID rguid) override;
};