#include "UCommon.h"
#include "UAnimatorStateHandler.h"


UAnimatorStateHandler::UAnimatorStateHandler(UAnimator *animator)
{
	mRefCount = 1;
	mListener = nullptr;
	mAnimator = animator;
}

UAnimatorStateHandler::~UAnimatorStateHandler()
{
}


void UAnimatorStateHandler::setOnAnimatorListener(UAnimator::OnAnimatorListener *l)
{
	mListener = l;
}


HRESULT STDMETHODCALLTYPE UAnimatorStateHandler::OnStoryboardStatusChanged(
	IUIAnimationStoryboard *storyboard,
	UI_ANIMATION_STORYBOARD_STATUS newStatus,
	UI_ANIMATION_STORYBOARD_STATUS previousStatus)
{
	if (mListener)
	{
		switch (newStatus)
		{
			//���ڴ�����
		case UI_ANIMATION_STORYBOARD_BUILDING:
			break;
			//׼�����š�
		case UI_ANIMATION_STORYBOARD_SCHEDULED:
			break;
			//��ȡ����
		case UI_ANIMATION_STORYBOARD_CANCELLED:
			mListener->onAnimationCancel(mAnimator);
			break;
			//���ڲ��š�
		case UI_ANIMATION_STORYBOARD_PLAYING:
			mListener->onAnimationStart(mAnimator);
			break;
			//�������ض̡�
		case UI_ANIMATION_STORYBOARD_TRUNCATED:
			break;
			//������ϡ�
		case UI_ANIMATION_STORYBOARD_FINISHED:
			mListener->onAnimationEnd(mAnimator);
			break;
			//������ϣ����Բ��š�
		case UI_ANIMATION_STORYBOARD_READY:
			break;
			//Ȩ�޲��㡣
		case UI_ANIMATION_STORYBOARD_INSUFFICIENT_PRIORITY:
			break;
		}
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE UAnimatorStateHandler::OnStoryboardUpdated(
	IUIAnimationStoryboard *storyboard)
{
	return S_OK;
}


IFACEMETHODIMP_(ULONG) UAnimatorStateHandler::AddRef()
{
	return InterlockedIncrement(&mRefCount);
}

IFACEMETHODIMP_(ULONG) UAnimatorStateHandler::Release()
{
	unsigned long newCount = InterlockedDecrement(&mRefCount);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

IFACEMETHODIMP UAnimatorStateHandler::QueryInterface(
	_In_ REFIID riid, _Outptr_ void** ppOutput)
{
	if (__uuidof(IUIAnimationStoryboardEventHandler) == riid)
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