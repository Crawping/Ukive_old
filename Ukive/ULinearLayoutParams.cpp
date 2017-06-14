#include "ULinearLayoutParams.h"


ULinearLayoutParams::ULinearLayoutParams(int width, int height)
	:ULayoutParams(width, height)
{
	weight = 0;
}

ULinearLayoutParams::ULinearLayoutParams(ULayoutParams *lp)
	: ULayoutParams(lp)
{
	weight = 0;
}


ULinearLayoutParams::~ULinearLayoutParams()
{
}
