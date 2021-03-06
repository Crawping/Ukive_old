#include "UCommon.h"
#include "UWordBreaker.h"

std::vector<std::shared_ptr<UWordBreaker::LanguageInfo>> 
UWordBreaker::sLangInfoList;

UWordBreaker::UWordBreaker()
{
	mIsReady = false;
}

UWordBreaker::~UWordBreaker()
{
}


void UWordBreaker::initGlobal()
{
	sLangInfoList.clear();

	HKEY languageKey = nullptr;
	LPCWSTR keyPath = L"SYSTEM\\CurrentControlSet\\Control\\ContentIndex\\Language";
	::RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &languageKey);
	if (languageKey != nullptr)
	{
		LSTATUS result;
		DWORD index = 0;
		DWORD subKeyNameSize = 260;
		wchar_t subKeyName[260];

		while ((result = ::RegEnumKeyExW(
			languageKey, index, subKeyName, &subKeyNameSize,
			nullptr, nullptr, nullptr, nullptr)) == ERROR_SUCCESS)
		{
			bool canPushBack = false;
			std::shared_ptr<LanguageInfo> langInfo(new LanguageInfo());
			langInfo->name = subKeyName;

			DWORD locale;
			DWORD dataSize = sizeof(DWORD);
			if (::RegGetValueW(
				languageKey, subKeyName,
				L"Locale", RRF_RT_REG_DWORD,
				nullptr, &locale, &dataSize) == ERROR_SUCCESS)
			{
				canPushBack = true;
				langInfo->locale = locale;
			}

			if (::RegGetValueW(
				languageKey, subKeyName,
				L"NoiseFile", RRF_RT_REG_SZ,
				nullptr, nullptr, &dataSize) == ERROR_SUCCESS)
			{
				wchar_t *noiseFile = new wchar_t[dataSize];

				if (::RegGetValueW(
					languageKey, subKeyName,
					L"NoiseFile", RRF_RT_REG_SZ,
					nullptr, noiseFile, &dataSize) == ERROR_SUCCESS)
				{
					langInfo->noiseFile = noiseFile;
				}

				delete noiseFile;
			}

			if (::RegGetValueW(
				languageKey, subKeyName,
				L"StemmerClass", RRF_RT_REG_SZ,
				nullptr, nullptr, &dataSize) == ERROR_SUCCESS)
			{
				wchar_t *stemmerClass = new wchar_t[dataSize];

				if (::RegGetValueW(
					languageKey, subKeyName,
					L"StemmerClass", RRF_RT_REG_SZ,
					nullptr, stemmerClass, &dataSize) == ERROR_SUCCESS)
				{
					langInfo->stemmerClass = stemmerClass;
				}

				delete stemmerClass;
			}

			if (::RegGetValueW(
				languageKey, subKeyName,
				L"WBreakerClass", RRF_RT_REG_SZ,
				nullptr, nullptr, &dataSize) == ERROR_SUCCESS)
			{
				wchar_t *wBreakerClass = new wchar_t[dataSize];

				if (::RegGetValueW(
					languageKey, subKeyName,
					L"WBreakerClass", RRF_RT_REG_SZ,
					nullptr, wBreakerClass, &dataSize) == ERROR_SUCCESS)
				{
					canPushBack = true;
					langInfo->wBreakerClass = wBreakerClass;
				}
				else
					canPushBack = false;

				delete wBreakerClass;
			}

			if (canPushBack)
				sLangInfoList.push_back(langInfo);

			subKeyNameSize = 260;
			++index;
		}

		::RegCloseKey(languageKey);
	}
}

void UWordBreaker::closeGlobal()
{
	sLangInfoList.clear();
}


bool UWordBreaker::loadWordBreaker(std::size_t index)
{
	std::shared_ptr<LanguageInfo> info 
		= sLangInfoList.at(index);
	std::wstring wordBreakerClass = info->wBreakerClass;

	CLSID clsidWordBreaker;
	HRESULT hr = CLSIDFromString(wordBreakerClass.c_str(), &clsidWordBreaker);
	if (SUCCEEDED(hr))
	{
		hr = ::CoCreateInstance(
			clsidWordBreaker, nullptr, CLSCTX_INPROC_SERVER,
			__uuidof(IWordBreaker), (void**)&mWordBreaker);
		if (SUCCEEDED(hr))
		{
			BOOL licence = TRUE;
			hr = mWordBreaker->Init(TRUE, 2000, &licence);
			if (SUCCEEDED(hr))
			{
				mIsReady = true;
				return true;
			}
		}
	}

	return false;
}


bool UWordBreaker::init()
{
	if (sLangInfoList.empty())
		return false;

	mIsReady = false;

	wchar_t *buffer = new wchar_t[LOCALE_NAME_MAX_LENGTH];
	if (::GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH) == 0)
	{
		delete buffer;
		return false;
	}

	bool hitted = false;
	std::size_t index = 0;
	std::wstring localeName(buffer);

	for (auto it = sLangInfoList.begin();
		it != sLangInfoList.end(); ++it)
	{
		if ((*it)->name == localeName)
		{
			hitted = true;
			mLangInfoIndex = index;
			break;
		}
		++index;
	}

	if (!hitted)
		return false;

	return loadWordBreaker(mLangInfoIndex);
}

bool UWordBreaker::init(std::uint32_t locale)
{
	if (sLangInfoList.empty())
		return false;

	mIsReady = false;

	bool hitted = false;
	std::size_t index = 0;
	for (auto it = sLangInfoList.begin();
		it != sLangInfoList.end(); ++it)
	{
		if ((*it)->locale == locale)
		{
			hitted = true;
			mLangInfoIndex = index;
			break;
		}
		++index;
	}

	if (!hitted)
		return false;

	return loadWordBreaker(mLangInfoIndex);
}

bool UWordBreaker::init(std::wstring localeName)
{
	if (sLangInfoList.empty())
		return false;

	mIsReady = false;

	bool hitted = false;
	std::size_t index = 0;
	for (auto it = sLangInfoList.begin();
		it != sLangInfoList.end(); ++it)
	{
		if ((*it)->name == localeName)
		{
			hitted = true;
			mLangInfoIndex = index;
			break;
		}
		++index;
	}

	if (!hitted)
		return false;

	return loadWordBreaker(mLangInfoIndex);
}

void UWordBreaker::close()
{
}


bool UWordBreaker::breakText(std::wstring text)
{
	return breakText(text, 0, text.length());
}

bool UWordBreaker::breakText(std::wstring text, ULONG start, ULONG end)
{
	if (!mIsReady)
		return false;

	TEXT_SOURCE textSource;
	textSource.iCur = start;
	textSource.iEnd = end;
	textSource.awcBuffer = text.c_str();
	textSource.pfnFillTextBuffer = TextBufferFiller::fillTextBuffer;

	if (mWordStoreSink == nullptr)
		mWordStoreSink = new WordStoreSink();
	mWordStoreSink->clearWords();

	HRESULT hr = mWordBreaker->BreakText(
		&textSource, mWordStoreSink.get(), nullptr);
	if (SUCCEEDED(hr))
	{
		return true;
	}

	return false;
}

std::size_t UWordBreaker::getWordCount()
{
	return mWordStoreSink->getWordCount();
}

std::wstring UWordBreaker::getWord(std::size_t index, std::uint32_t *start)
{
	std::shared_ptr<WordStoreSink::WordBlock> wb
		= mWordStoreSink->getWord(index);
	if (start != nullptr)
		*start = wb->start;

	return wb->word;
}



WordStoreSink::WordStoreSink()
{
	mRefCount = 1;
}

void WordStoreSink::clearWords()
{
	mWordsList.clear();
}

std::size_t WordStoreSink::getWordCount()
{
	return mWordsList.size();
}

std::shared_ptr<WordStoreSink::WordBlock> 
WordStoreSink::getWord(std::size_t index)
{
	return mWordsList.at(index);
}

HRESULT STDMETHODCALLTYPE WordStoreSink::PutWord(
	ULONG cwc, const WCHAR *pwcInBuf,
	ULONG cwcSrcLen, ULONG cwcSrcPos)
{
	std::shared_ptr<WordBlock> wordBlock(new WordBlock());
	wordBlock->start = cwcSrcPos;
	wordBlock->word = std::wstring(pwcInBuf, cwc);

	mWordsList.push_back(wordBlock);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WordStoreSink::PutAltWord(
	ULONG cwc, const WCHAR *pwcInBuf,
	ULONG cwcSrcLen, ULONG cwcSrcPos)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WordStoreSink::StartAltPhrase()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WordStoreSink::EndAltPhrase()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WordStoreSink::PutBreak(
	WORDREP_BREAK_TYPE breakType)
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE WordStoreSink::AddRef()
{
	return InterlockedIncrement(&mRefCount);
}

ULONG STDMETHODCALLTYPE WordStoreSink::Release()
{
	LONG cr = InterlockedDecrement(&mRefCount);

	if (mRefCount == 0)
		delete this;

	return cr;
}

HRESULT STDMETHODCALLTYPE WordStoreSink::QueryInterface(
	REFIID riid, void **ppvObject)
{
	if (ppvObject == nullptr)
		return E_INVALIDARG;

	*ppvObject = nullptr;

	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObject = reinterpret_cast<IUnknown*>(this);
	}
	else if (IsEqualIID(riid, __uuidof(IWordSink)))
	{
		*ppvObject = (IWordSink*)this;
	}

	if (*ppvObject)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}