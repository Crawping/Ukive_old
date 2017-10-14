#include "BaseLayoutParams.h"


BaseLayoutParams::BaseLayoutParams(int width, int height)
	:LayoutParams(width, height)
{
}

BaseLayoutParams::BaseLayoutParams(LayoutParams *lp)
	: LayoutParams(lp)
{
}

BaseLayoutParams::~BaseLayoutParams()
{
}
