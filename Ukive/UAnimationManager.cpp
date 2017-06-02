#include "UCommon.h"
#include "UAnimationManagerEventHandler.h"
#include "UAnimationTimerHandler.h"
#include "UAnimationManager.h"


IUIAnimationTransitionLibrary*
UAnimationManager::sTransitionLibrary = nullptr;
IUIAnimationTransitionFactory*
UAnimationManager::sTransitionFactory = nullptr;


UAnimationManager::UAnimationManager()
{
}


UAnimationManager::~UAnimationManager()
{
}


HRESULT UAnimationManager::init()
{
	HRESULT hr = ::CoCreateInstance(
		CLSID_UIAnimationManager,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&mAnimationManager));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = ::CoCreateInstance(
		CLSID_UIAnimationTimer,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&mAnimationTimer));
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void UAnimationManager::close()
{
}

HRESULT UAnimationManager::initGlobal()
{
	HRESULT hr = ::CoCreateInstance(
		CLSID_UIAnimationTransitionLibrary,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&sTransitionLibrary));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = ::CoCreateInstance(
		CLSID_UIAnimationTransitionFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&sTransitionFactory));
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void UAnimationManager::closeGlobal()
{
	sTransitionLibrary->Release();
	sTransitionLibrary = nullptr;

	sTransitionFactory->Release();
	sTransitionFactory = nullptr;
}


void UAnimationManager::pause()
{
	mAnimationManager->Pause();
}

void UAnimationManager::resume()
{
	mAnimationManager->Resume();
}

void UAnimationManager::finish(double second)
{
	mAnimationManager->FinishAllStoryboards(second);
}

void UAnimationManager::abandon()
{
	mAnimationManager->AbandonAllStoryboards();
}

bool UAnimationManager::isBusy()
{
	UI_ANIMATION_MANAGER_STATUS status;
	HRESULT hr = mAnimationManager->GetStatus(&status);

	return (status == UI_ANIMATION_MANAGER_BUSY);
}

bool UAnimationManager::update()
{
	UI_ANIMATION_SECONDS secondsNow;
	HRESULT hr = mAnimationTimer->GetTime(&secondsNow);

	if (SUCCEEDED(hr))
		hr = mAnimationManager->Update(secondsNow);

	return SUCCEEDED(hr);
}


void UAnimationManager::connectTimer(bool enable)
{
	HRESULT hr;

	if (enable)
	{
		IUIAnimationTimerUpdateHandler *pTimerUpdateHandler;
		hr = mAnimationManager->QueryInterface(IID_PPV_ARGS(&pTimerUpdateHandler));
		if (SUCCEEDED(hr))
		{
			hr = mAnimationTimer->SetTimerUpdateHandler(
				pTimerUpdateHandler,
				UI_ANIMATION_IDLE_BEHAVIOR_DISABLE);
			pTimerUpdateHandler->Release();
		}
	}
	else
	{
		hr = mAnimationTimer->SetTimerUpdateHandler(
			0, UI_ANIMATION_IDLE_BEHAVIOR_DISABLE);
	}
}


void UAnimationManager::setTimerEventListener(OnTimerEventListener *l)
{
	if (l == 0)
		mAnimationTimer->SetTimerEventHandler(0);
	else
	{
		UAnimationTimerHandler *handler = new UAnimationTimerHandler(l);
		mAnimationTimer->SetTimerEventHandler(handler);
		handler->Release();
	}
}

void UAnimationManager::setOnStateChangedListener(OnStateChangedListener *l)
{
	if (l == 0)
		mAnimationManager->SetManagerEventHandler(0);
	else
	{
		UAnimationManagerEventHandler *handler = new UAnimationManagerEventHandler(l);
		mAnimationManager->SetManagerEventHandler(handler);
		handler->Release();
	}
}


IUIAnimationManager *UAnimationManager::getAnimationManager()
{
	return mAnimationManager.get();
}

IUIAnimationTimer *UAnimationManager::getAnimationTimer()
{
	return mAnimationTimer.get();
}

IUIAnimationTransitionLibrary *UAnimationManager::getTransitionLibrary()
{
	return sTransitionLibrary;
}