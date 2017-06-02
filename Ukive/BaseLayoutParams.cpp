#include "BaseLayoutParams.h"


BaseLayoutParams::BaseLayoutParams(int width, int height)
	:ULayoutParams(width, height)
{
	viewType = TYPE_NORMAL;
}

BaseLayoutParams::BaseLayoutParams(ULayoutParams *lp)
	: ULayoutParams(lp)
{
	viewType = TYPE_NORMAL;
}

BaseLayoutParams::~BaseLayoutParams()
{
}
