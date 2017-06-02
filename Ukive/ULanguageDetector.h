#pragma once
#include <elscore.h>
#include <elssrvc.h>

#pragma comment(lib, "Elscore.lib")


class ULanguageDetector
{
private:
	PMAPPING_SERVICE_INFO mService;

public:
	ULanguageDetector();
	~ULanguageDetector();

	bool init();
	void close();

	bool detect(std::wstring text);
	bool detect(std::wstring text, std::uint32_t start);
};