#include "LinearLayoutParams.h"


LinearLayoutParams::LinearLayoutParams(int width, int height)
	:ULayoutParams(width, height)
{
	weight = 0;
}

LinearLayoutParams::LinearLayoutParams(ULayoutParams *lp)
	: ULayoutParams(lp)
{
	weight = 0;
}


LinearLayoutParams::~LinearLayoutParams()
{
}
