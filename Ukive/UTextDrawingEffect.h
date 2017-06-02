#pragma once

class EffectSpan;

class DECLSPEC_UUID("06C2F086-4818-4DF9-880D-FF7C9F796AFA") UTextDrawingEffect : public IUnknown
{
private:
	ULONG mRefCount;

public:
	UTextDrawingEffect();
	~UTextDrawingEffect();

	EffectSpan *mEffectSpan;

public:
	unsigned long STDMETHODCALLTYPE AddRef();
	unsigned long STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(
		IID const& riid,
		void** ppvObject
	);
};

