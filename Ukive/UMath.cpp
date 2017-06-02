#include <math.h>
#include <cstdlib>
#include "UMath.h"

int UMath::maximum(int i1, int i2)
{
	return static_cast<int>(::fmaxf(
		static_cast<float>(i1),
		static_cast<float>(i2)));
}

float UMath::maximum(float f1, float f2)
{
	return ::fmaxf(f1, f2);
}

double UMath::maximum(double d1, double d2)
{
	return ::fmax(d1, d2);
}


int UMath::minimum(int i1, int i2)
{
	return static_cast<int>(::fminf(
		static_cast<float>(i1),
		static_cast<float>(i2)));
}

float UMath::minimum(float f1, float f2)
{
	return ::fminf(f1, f2);
}

double UMath::minimum(double d1, double d2)
{
	return ::fmin(d1, d2);
}


int UMath::abs(int value)
{
	return ::abs(value);
}

float UMath::abs(float value)
{
	return ::fabsf(value);
}

double UMath::abs(double value)
{
	return ::fabs(value);
}


float UMath::ceil(float f)
{
	return ::ceilf(f);
}

double UMath::ceil(double d)
{
	return ::ceil(d);
}

float UMath::floor(float f)
{
	return ::floorf(f);
}

double UMath::floor(double d)
{
	return ::floor(d);
}


float UMath::round(float f)
{
	return ::roundf(f);
}

double UMath::round(double d)
{
	return ::round(d);
}


float UMath::sqrt(float f)
{
	return ::sqrtf(f);
}

double UMath::sqrt(double d)
{
	return ::sqrt(d);
}


int UMath::pow(int base, int exp)
{
	return static_cast<int>(::powf(
		static_cast<float>(base), 
		static_cast<float>(exp)));
}

float UMath::pow(float base, float exp)
{
	return ::powf(base, exp);
}

double UMath::pow(double base, double exp)
{
	return ::pow(base, exp);
}

long long UMath::pow(long long base, long long exp)
{
	return static_cast<long long>(::pow(
		static_cast<double>(base),
		static_cast<double>(exp)));
}


int UMath::random()
{
	return std::rand();
}

int UMath::random(unsigned int seed)
{
	std::srand(seed);
	return std::rand();
}