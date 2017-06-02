#pragma once
#include "UAnimationManager.h"

class UAnimationTimerHandler : public IUIAnimationTimerEventHandler
{
private:
	ULONG mRefCount;
	UAnimationManager::OnTimerEventListener *mListener;

public:
	UAnimationTimerHandler(
		UAnimationManager::OnTimerEventListener *listener);
	~UAnimationTimerHandler();

	HRESULT STDMETHODCALLTYPE OnPreUpdate();
	HRESULT STDMETHODCALLTYPE OnPostUpdate();
	HRESULT STDMETHODCALLTYPE OnRenderingTooSlow(
			UINT32 framesPerSecond);

	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);
};