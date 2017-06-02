#include "UCommon.h"
#include "UAnimationManagerEventHandler.h"


UAnimationManagerEventHandler::UAnimationManagerEventHandler(
	UAnimationManager::OnStateChangedListener *listener)
{
	mRefCount = 1;
	mListener = listener;
}


UAnimationManagerEventHandler::~UAnimationManagerEventHandler()
{
}


HRESULT STDMETHODCALLTYPE UAnimationManagerEventHandler::OnManagerStatusChanged(
	_In_  UI_ANIMATION_MANAGER_STATUS newStatus,
	_In_  UI_ANIMATION_MANAGER_STATUS previousStatus)
{
	if (mListener)
		mListener->onStateChanged(newStatus, previousStatus);

	return S_OK;
}


IFACEMETHODIMP_(ULONG) UAnimationManagerEventHandler::AddRef()
{
	return InterlockedIncrement(&mRefCount);
}

IFACEMETHODIMP_(ULONG) UAnimationManagerEventHandler::Release()
{
	unsigned long newCount = InterlockedDecrement(&mRefCount);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

IFACEMETHODIMP UAnimationManagerEventHandler::QueryInterface(
	_In_ REFIID riid, _Outptr_ void** ppOutput)
{
	if (__uuidof(IUIAnimationManagerEventHandler) == riid)
	{
		*ppOutput = this;
	}
	else if (__uuidof(IUnknown) == riid)
	{
		*ppOutput = this;
	}
	else
	{
		*ppOutput = 0;
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}