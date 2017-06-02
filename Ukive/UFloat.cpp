#include "UCommon.h"
#include "UFloat.h"


UFloat::UFloat(float value)
{
	mFu.rawValue = value;
}

UFloat::~UFloat()
{
}


bool UFloat::isNaN()
{
	return ((mFu.bits & sBaseMask) != 0Ui32
		&& (mFu.bits & sExpoMask) == sExpoMask);
}

bool UFloat::isInfinity()
{
	return ((mFu.bits & sBaseMask) == 0Ui32
		&& (mFu.bits & sExpoMask) == sExpoMask);
}

bool UFloat::isNegative()
{
	return (mFu.bits & sSignMask) != 0Ui32;
}

uint32_t UFloat::exponent()
{
	return mFu.bits & sExpoMask;
}

uint32_t UFloat::mantissa()
{
	return mFu.bits & sBaseMask;
}


int UFloat::compareTo(const UFloat& f)
{
	std::uint32_t lhs = mFu.bits;
	std::uint32_t rhs = f.mFu.bits;

	if (lhs & sSignMask)
		lhs = ~lhs + 1Ui32;
	else
		lhs |= sSignMask;

	if (rhs & sSignMask)
		rhs = ~rhs + 1Ui32;
	else
		rhs |= sSignMask;

	if (lhs >= rhs && (lhs - rhs) > sULPs)
		return 1;
	else if (lhs < rhs && (rhs - lhs) > sULPs)
		return -1;
	else
		return 0;
}


std::wstring UFloat::toString()
{
	return std::to_wstring(mFu.rawValue);
}

std::wstring UFloat::toString(int decimalCount)
{
	std::wstring text = std::to_wstring(mFu.rawValue);
	size_t index = text.find('.');
	if (index == std::wstring::npos)
		return text.append(L".00");
	else
	{
		if (index + decimalCount + 1 >= text.length())
			return text;
		else
			return text.substr(0, index + decimalCount + 1);
	}
}


int UFloat::compare(float lhs, float rhs)
{
	return UFloat(lhs).compareTo(UFloat(rhs));
}

float UFloat::parseFloat(std::wstring s)
{
	if (s.empty())
		throw std::invalid_argument("null");

	return 0;
}

std::wstring UFloat::toString(float value)
{
	return UFloat(value).toString();
}

std::wstring UFloat::toString(float value, int decimalCount)
{
	return UFloat(value).toString(decimalCount);
}