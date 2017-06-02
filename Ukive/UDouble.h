#pragma once
#include <limits>
#include <cstdint>
#include <string>

///<summary>
///根据IEEE 754标准实现的有关double的相关方法。
///</summary>
class UDouble
{
private:
	union DoubleUnion
	{
		double rawValue;
		uint64_t bits;
	};

	static const uint64_t sULPs = 4Ui64;

	static const uint64_t sSignMask = 1Ui64 << 63Ui64;
	static const uint64_t sBaseMask = 0xFFFFFFFFFFFFFUi64;
	static const uint64_t sExpoMask = ~(sSignMask | sBaseMask);

private:
	DoubleUnion mDu;

public:
	explicit UDouble(double value);
	~UDouble();

	bool isNaN();
	bool isInfinity();
	bool isNegative();

	uint64_t exponent();
	uint64_t mantissa();

	int compareTo(const UDouble& f);

	std::wstring toString();
	std::wstring toString(int decimalCount);

	static int compare(double lhs, double rhs);
	static float parseDouble(std::wstring s);
	static std::wstring toString(double value);
};