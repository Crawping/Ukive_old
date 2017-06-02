#pragma once
#include "UAnimator.h"

class UAnimatorStateHandler : public IUIAnimationStoryboardEventHandler
{
private:
	ULONG mRefCount;
	UAnimator *mAnimator;
	UAnimator::OnAnimatorListener *mListener;

public:
	UAnimatorStateHandler(UAnimator *animator);
	~UAnimatorStateHandler();

	void setOnAnimatorListener(UAnimator::OnAnimatorListener *l);

	HRESULT STDMETHODCALLTYPE OnStoryboardStatusChanged(
		IUIAnimationStoryboard *storyboard,
		UI_ANIMATION_STORYBOARD_STATUS newStatus,
		UI_ANIMATION_STORYBOARD_STATUS previousStatus);

	HRESULT STDMETHODCALLTYPE OnStoryboardUpdated(
			IUIAnimationStoryboard *storyboard);

	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);
};

