#pragma once
#include <limits>
#include <cstdint>
#include <string>

///<summary>
///根据IEEE 754标准实现的有关float的相关方法。
///</summary>
class UFloat
{
private:
	union FloatUnion
	{
		float rawValue;
		uint32_t bits;
	};

	static const uint32_t sULPs = 4U;

	static const uint32_t sSignMask = 1U << 31U;
	static const uint32_t sBaseMask = 0x007FFFFFU;
	static const uint32_t sExpoMask = ~(sSignMask | sBaseMask);

private:
	FloatUnion mFu;

public:
	explicit UFloat(float value);
	~UFloat();

	bool isNaN();
	bool isInfinity();
	bool isNegative();

	uint32_t exponent();
	uint32_t mantissa();

	int compareTo(const UFloat& f);

	std::wstring toString();
	std::wstring toString(int decimalCount);

	static int compare(float lhs, float rhs);
	static float parseFloat(std::wstring s);
	static std::wstring toString(float value);
	static std::wstring toString(float value, int decimalCount);
};