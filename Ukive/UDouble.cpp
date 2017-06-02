#include "UCommon.h"
#include "UDouble.h"


UDouble::UDouble(double value)
{
	mDu.rawValue = value;
}

UDouble::~UDouble()
{
}


bool UDouble::isNaN()
{
	return ((mDu.bits & sBaseMask) != 0Ui64
		&& (mDu.bits & sExpoMask) == sExpoMask);
}

bool UDouble::isInfinity()
{
	return ((mDu.bits & sBaseMask) == 0Ui64
		&& (mDu.bits & sExpoMask) == sExpoMask);
}

bool UDouble::isNegative()
{
	return (mDu.bits & sSignMask) != 0Ui64;
}


uint64_t UDouble::exponent()
{
	return mDu.bits & sExpoMask;
}

uint64_t UDouble::mantissa()
{
	return mDu.bits & sBaseMask;
}

int UDouble::compareTo(const UDouble& f)
{
	std::uint64_t lhs = mDu.bits;
	std::uint64_t rhs = f.mDu.bits;

	if (lhs & sSignMask)
		lhs = ~lhs + 1Ui64;
	else
		lhs |= sSignMask;

	if (rhs & sSignMask)
		rhs = ~rhs + 1Ui64;
	else
		rhs |= sSignMask;

	if (lhs >= rhs && (lhs - rhs) > sULPs)
		return 1;
	else if (lhs < rhs && (rhs - lhs) > sULPs)
		return -1;
	else
		return 0;
}

std::wstring UDouble::toString()
{
	std::wstringstream ss;
	ss << mDu.rawValue;

	return ss.str();
}

std::wstring UDouble::toString(int decimalCount)
{
	std::wstringstream ss;
	ss << mDu.rawValue;

	return ss.str();
}

int UDouble::compare(double lhs, double rhs)
{
	return UDouble(lhs).compareTo(UDouble(rhs));
}

float UDouble::parseDouble(std::wstring s)
{
	if (s.empty())
		throw std::invalid_argument("null");

	return 0;
}

std::wstring UDouble::toString(double value)
{
	return UDouble(value).toString();
}