#include "LinearLayoutParams.h"


LinearLayoutParams::LinearLayoutParams(int width, int height)
	:LayoutParams(width, height)
{
	weight = 0;
}

LinearLayoutParams::LinearLayoutParams(LayoutParams *lp)
	: LayoutParams(lp)
{
	weight = 0;
}


LinearLayoutParams::~LinearLayoutParams()
{
}
