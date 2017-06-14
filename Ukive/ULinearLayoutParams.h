#pragma once
#include "ULayoutParams.h"

class ULinearLayoutParams : public ULayoutParams
{
public:
	int weight;

public:
	ULinearLayoutParams(int width, int height);
	ULinearLayoutParams(ULayoutParams *lp);
	virtual ~ULinearLayoutParams();
};