#pragma once
#include "UAnimationManager.h"

class UAnimationManagerEventHandler : public IUIAnimationManagerEventHandler
{
private:
	ULONG mRefCount;
	UAnimationManager::OnStateChangedListener *mListener;

public:
	UAnimationManagerEventHandler(
		UAnimationManager::OnStateChangedListener *listener);
	~UAnimationManagerEventHandler();

	HRESULT STDMETHODCALLTYPE OnManagerStatusChanged(
		_In_  UI_ANIMATION_MANAGER_STATUS newStatus,
		_In_  UI_ANIMATION_MANAGER_STATUS previousStatus);

	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);
};