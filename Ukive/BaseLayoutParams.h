#pragma once
#include "LayoutParams.h"

class BaseLayoutParams : public LayoutParams
{
public:
	BaseLayoutParams(int width, int height);
	BaseLayoutParams(LayoutParams *lp);
	~BaseLayoutParams();
};