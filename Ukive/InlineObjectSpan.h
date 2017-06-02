#pragma once
#include "USpan.h"

class InlineObjectSpan : public USpan
{
public:
	InlineObjectSpan(unsigned int start, unsigned int end);
	virtual ~InlineObjectSpan();

	int getBaseType();
};