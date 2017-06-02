#include "UCommon.h"
#include "UAnimationTimerHandler.h"


UAnimationTimerHandler::UAnimationTimerHandler(
	UAnimationManager::OnTimerEventListener *listener)
{
	mRefCount = 1;
	mListener = listener;
}


UAnimationTimerHandler::~UAnimationTimerHandler()
{
}


HRESULT STDMETHODCALLTYPE UAnimationTimerHandler::OnPreUpdate()
{
	if (mListener)
		mListener->OnPreUpdate();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE UAnimationTimerHandler::OnPostUpdate()
{
	if (mListener)
		mListener->OnPostUpdate();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE UAnimationTimerHandler::OnRenderingTooSlow(
	UINT32 framesPerSecond)
{
	if (mListener)
		mListener->OnRenderingTooSlow(framesPerSecond);

	return S_OK;
}


IFACEMETHODIMP_(ULONG) UAnimationTimerHandler::AddRef()
{
	return InterlockedIncrement(&mRefCount);
}

IFACEMETHODIMP_(ULONG) UAnimationTimerHandler::Release()
{
	unsigned long newCount = InterlockedDecrement(&mRefCount);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

IFACEMETHODIMP UAnimationTimerHandler::QueryInterface(
	_In_ REFIID riid, _Outptr_ void** ppOutput)
{
	if (__uuidof(IUIAnimationTimerEventHandler) == riid)
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