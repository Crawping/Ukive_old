#pragma once
#include "ULayoutParams.h"

class UBaseLayoutParams : public ULayoutParams
{
public:
	UBaseLayoutParams(int width, int height);
	UBaseLayoutParams(ULayoutParams *lp);
	~UBaseLayoutParams();
};