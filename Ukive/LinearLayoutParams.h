#pragma once
#include "LayoutParams.h"

class LinearLayoutParams : public LayoutParams
{
public:
	int weight;

public:
	LinearLayoutParams(int width, int height);
	LinearLayoutParams(LayoutParams *lp);
	virtual ~LinearLayoutParams();
};