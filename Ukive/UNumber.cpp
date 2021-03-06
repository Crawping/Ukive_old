#include "UCommon.h"
#include "UNumber.h"


wchar_t UNumber::mDigitc[] = 
{
	L'0',L'1',L'2',L'3',L'4',
	L'5',L'6',L'7',L'8',L'9',
	L'a',L'b',L'c',L'd',L'e',
	L'f',L'g',L'h',L'i',L'j',
	L'k',L'l',L'm',L'n',L'o',
	L'p',L'q',L'r',L's',L't',
	L'u',L'v',L'w',L'x',L'y',L'z'
};

int32_t UNumber::digit(wchar_t code, int radix)
{
	if (code < 128) 
	{
		int result = -1;
		if (L'0' <= code && code <= L'9')
			result = code - L'0';

		else if (L'a' <= code && code <= L'z')
			result = 10 + (code - L'a');

		else if (L'A' <= code && code <= L'Z')
			result = 10 + (code - L'A');

		return result < radix ? result : -1;
	}

	return -1;
}

wchar_t UNumber::character(int32_t number)
{
	if (number >= 0 && number <= 9)
		return number + L'0';

	return -1;
}


int32_t UNumber::parseInt(std::wstring s, int radix)
{
	if (s.empty())
		throw std::invalid_argument("null");
	if (radix < MIN_RADIX || radix > MAX_RADIX)
		throw std::invalid_argument("illegal radix.");

	int32_t result = 0;
	int32_t digit;
	int32_t multmin;
	size_t i = 0, len = s.length();
	int32_t limit = -(std::numeric_limits<int32_t>::max)();
	bool negative = false;

	wchar_t firstChar = s.at(0);
	if (firstChar < L'0')
	{
		if (firstChar == L'-')
		{
			negative = true;
			limit = (std::numeric_limits<int32_t>::min)();
		}
		else if (firstChar != L'+')
			throw std::runtime_error("illegal number str");
		i++;
	}
	multmin = limit / radix;

	while (i < len)
	{
		digit = UNumber::digit(s.at(i++), radix);
		if (digit < 0)
			throw std::runtime_error("illegal number str");
		if (result < multmin)
			throw std::runtime_error("illegal number str");

		result *= radix;
		if (result < limit + digit)
			throw std::runtime_error("illegal number str");

		result -= digit;
	}

	return negative ? result : -result;
}

int64_t UNumber::parseLong(std::wstring s, int radix)
{
	if (s.empty())
		throw std::invalid_argument("null");
	if (radix < MIN_RADIX || radix > MAX_RADIX)
		throw std::invalid_argument("radix is illegal.");

	int32_t digit;
	size_t i = 0, len = s.length();
	int64_t multmin;
	int64_t result = 0;
	int64_t limit = -(std::numeric_limits<int64_t>::max)();
	bool negative = false;

	wchar_t firstChar = s.at(0);
	if (firstChar < L'0')
	{
		if (firstChar == L'-')
		{
			negative = true;
			limit = (std::numeric_limits<int64_t>::min)();
		}
		else if (firstChar != L'+')
			throw std::runtime_error("illegal number str");
		i++;
	}
	multmin = limit / radix;

	while (i < len)
	{
		digit = UNumber::digit(s.at(i++), radix);
		if (digit < 0)
			throw std::runtime_error("illegal number str");
		if (result < multmin)
			throw std::runtime_error("illegal number str");

		result *= radix;
		if (result < limit + digit)
			throw std::runtime_error("illegal number str");

		result -= digit;
	}

	return negative ? result : -result;
}


std::wstring UNumber::toString(int32_t value)
{
	bool isNeg = false;
	if (value < 0)
	{
		value = -value;
		isNeg = true;
	}

	wchar_t result[12];
	result[11] = L'\0';

	int index = 10;
	int32_t number;
	int32_t remain = value;

	do
	{
		number = remain % 10;
		remain = remain / 10;

		result[index--] = character(number);
	} while (remain != 0);

	if (isNeg)
		result[index--] = L'-';

	return std::wstring(result + index + 1);
}

std::wstring UNumber::toString(uint32_t value)
{
	wchar_t result[11];
	result[10] = L'\0';

	int index = 9;
	int32_t number;
	uint32_t remain = value;

	do
	{
		number = remain % 10;
		remain = remain / 10;

		result[index--] = character(number);
	} while (remain != 0);

	return std::wstring(result + index + 1);
}

std::wstring UNumber::toString(int64_t value)
{
	bool isNeg = false;
	if (value < 0)
	{
		value = -value;
		isNeg = true;
	}

	wchar_t result[21];
	result[20] = L'\0';

	int index = 19;
	int32_t number;
	int64_t remain = value;

	do
	{
		number = remain % 10;
		remain = remain / 10;

		result[index--] = character(number);
	} while (remain != 0);

	if (isNeg)
		result[index--] = L'-';

	return std::wstring(result + index + 1);
}

std::wstring UNumber::toString(uint64_t value)
{
	wchar_t result[11];
	result[10] = L'\0';

	int index = 9;
	int32_t number;
	uint64_t remain = value;

	do
	{
		number = remain % 10;
		remain = remain / 10;

		result[index--] = character(number);
	} while (remain != 0);

	return std::wstring(result + index + 1);
}