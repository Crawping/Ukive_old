#pragma once
#include "ULayoutParams.h"

class LinearLayoutParams : public ULayoutParams
{
public:
	int weight;

public:
	LinearLayoutParams(int width, int height);
	LinearLayoutParams(ULayoutParams *lp);
	virtual ~LinearLayoutParams();
};