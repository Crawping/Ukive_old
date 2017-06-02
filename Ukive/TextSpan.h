#pragma once
#include "USpan.h"

class TextSpan : public USpan
{
public:
	TextSpan(unsigned int start, unsigned int end);
	virtual ~TextSpan();

	int getBaseType();
};