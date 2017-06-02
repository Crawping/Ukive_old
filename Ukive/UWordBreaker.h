#pragma once
#include "UComPtr.h"
#include <CiError.h>
#include <indexsrv.h>

class WordStoreSink;

class UWordBreaker
{
private:
	struct LanguageInfo
	{
		DWORD locale;
		std::wstring name;
		std::wstring noiseFile;
		std::wstring stemmerClass;
		std::wstring wBreakerClass;

		LanguageInfo()
		{
			locale = 0;
			name = L"";
			noiseFile = L"";
			stemmerClass = L"";
			wBreakerClass = L"";
		}
	};

	class TextBufferFiller
	{
	public:
		static long __stdcall fillTextBuffer(TEXT_SOURCE *pTextSource)
		{
			return WBREAK_E_END_OF_TEXT;
		}
	};

	static std::vector<std::shared_ptr<LanguageInfo>> sLangInfoList;

	bool mIsReady;
	std::size_t mLangInfoIndex;
	UComPtr<IWordBreaker> mWordBreaker;
	UComPtr<WordStoreSink> mWordStoreSink;

	bool loadWordBreaker(std::size_t index);

public:
	UWordBreaker();
	~UWordBreaker();

	static void initGlobal();
	static void closeGlobal();

	bool init();
	bool init(std::uint32_t locale);
	bool init(std::wstring localeName);
	void close();

	bool breakText(std::wstring text);
	bool breakText(std::wstring text, ULONG start, ULONG end);

	std::size_t getWordCount();
	std::wstring getWord(std::size_t index, std::uint32_t *start = nullptr);
};


class WordStoreSink : public IWordSink
{
public:
	struct WordBlock
	{
		std::uint32_t start;
		std::wstring word;
	};

private:
	ULONG mRefCount;
	std::vector<std::shared_ptr<WordBlock>> mWordsList;

public:
	WordStoreSink();

	void clearWords();
	std::size_t getWordCount();
	std::shared_ptr<WordBlock> getWord(std::size_t index);

	HRESULT STDMETHODCALLTYPE PutWord(
		ULONG cwc,const WCHAR *pwcInBuf,
		ULONG cwcSrcLen,ULONG cwcSrcPos) override;

	HRESULT STDMETHODCALLTYPE PutAltWord(
		ULONG cwc,const WCHAR *pwcInBuf,
		ULONG cwcSrcLen,ULONG cwcSrcPos) override;

	HRESULT STDMETHODCALLTYPE StartAltPhrase() override;

	HRESULT STDMETHODCALLTYPE EndAltPhrase() override;

	HRESULT STDMETHODCALLTYPE PutBreak(
		WORDREP_BREAK_TYPE breakType) override;

	ULONG STDMETHODCALLTYPE AddRef() override;
	ULONG STDMETHODCALLTYPE Release() override;
	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID riid, void **ppvObject) override;
};