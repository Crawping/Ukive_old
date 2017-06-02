#pragma once
#include "ULayoutParams.h"

class BaseLayoutParams : public ULayoutParams
{
public:
	int viewType;

	const static int TYPE_NORMAL = 0;
	const static int TYPE_MENU = 1;
	const static int TYPE_DIALOG = 2;

public:
	BaseLayoutParams(int width, int height);
	BaseLayoutParams(ULayoutParams *lp);
	~BaseLayoutParams();
};