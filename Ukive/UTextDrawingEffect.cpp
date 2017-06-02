#include "UCommon.h"
#include "EffectSpan.h"
#include "UTextDrawingEffect.h"


UTextDrawingEffect::UTextDrawingEffect()
	:mRefCount(1)
{
	mEffectSpan = 0;
}


UTextDrawingEffect::~UTextDrawingEffect()
{
}


STDMETHODIMP_(unsigned long) UTextDrawingEffect::AddRef()
{
	return InterlockedIncrement(&mRefCount);
}


STDMETHODIMP_(unsigned long) UTextDrawingEffect::Release()
{
	unsigned long newCount = InterlockedDecrement(&mRefCount);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}


STDMETHODIMP UTextDrawingEffect::QueryInterface(
	IID const& riid,
	void** ppvObject
)
{
	if (__uuidof(UTextDrawingEffect) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IUnknown) == riid)
	{
		*ppvObject = this;
	}
	else
	{
		*ppvObject = NULL;
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}