#pragma once

class UMath
{
public:
	static int maximum(int i1, int i2);
	static float maximum(float f1, float f2);
	static double maximum(double d1, double d2);

	static int minimum(int i1, int i2);
	static float minimum(float f1, float f2);
	static double minimum(double d1, double d2);

	static int abs(int value);
	static float abs(float value);
	static double abs(double value);

	//向正无穷大舍入。
	//如: ceil(10.5) == 11  ceil(-10.5) ==-10
	static float ceil(float f);

	//向正无穷大舍入。
	//如: ceil(10.5) == 11  ceil(-10.5) ==-10
	static double ceil(double d);

	//向负无穷大舍入。
	//如: floor(10.5) == 10  floor(-10.5) == -11
	static float floor(float f);

	//向负无穷大舍入。
	//如: floor(10.5) == 10  floor(-10.5) == -11
	static double floor(double d);

	//四舍五入。
	static float round(float f);

	//四舍五入。
	static double round(double d);

	static float sqrt(float f);
	static double sqrt(double f);

	static int pow(int base, int exp);
	static float pow(float base, float exp);
	static double pow(double base, double exp);
	static long long pow(long long base, long long exp);

	static int random();
	static int random(unsigned int seed);
};