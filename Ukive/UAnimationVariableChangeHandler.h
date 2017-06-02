#pragma once
#include "UAnimator.h"

class UAnimationVariableChangeHandler : public IUIAnimationVariableChangeHandler, public IUIAnimationVariableIntegerChangeHandler
{
private:
	ULONG mRefCount;
	unsigned int mVarIndex;
	UAnimator::OnValueChangedListener *mListener;

public:
	UAnimationVariableChangeHandler(
		UAnimator::OnValueChangedListener *listener, unsigned int varIndex);
	~UAnimationVariableChangeHandler();

	HRESULT STDMETHODCALLTYPE OnValueChanged(
		_In_  IUIAnimationStoryboard *storyboard,
		_In_  IUIAnimationVariable *variable,
		_In_  DOUBLE newValue,
		_In_  DOUBLE previousValue);

	HRESULT STDMETHODCALLTYPE OnIntegerValueChanged(
		_In_  IUIAnimationStoryboard *storyboard,
		_In_  IUIAnimationVariable *variable,
		_In_  INT32 newValue,
		_In_  INT32 previousValue);

	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);
};