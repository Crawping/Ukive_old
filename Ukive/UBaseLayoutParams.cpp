#include "UBaseLayoutParams.h"


UBaseLayoutParams::UBaseLayoutParams(int width, int height)
	:ULayoutParams(width, height)
{
}

UBaseLayoutParams::UBaseLayoutParams(ULayoutParams *lp)
	: ULayoutParams(lp)
{
}

UBaseLayoutParams::~UBaseLayoutParams()
{
}
