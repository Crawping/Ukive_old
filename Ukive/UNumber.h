#pragma once
#include <limits>

class UNumber
{
private:
	static wchar_t mDigitc[];

public:
	static const int MIN_RADIX = 2;
	static const int MAX_RADIX = 36;

	static int32_t digit(wchar_t code, int radix);
	static wchar_t character(int32_t number);

	static int32_t parseInt(std::wstring s, int radix);
	static int64_t parseLong(std::wstring s, int radix);

	static std::wstring toString(int32_t value);
	static std::wstring toString(uint32_t value);
	static std::wstring toString(int64_t value);
	static std::wstring toString(uint64_t value);

private:
	UNumber();
};