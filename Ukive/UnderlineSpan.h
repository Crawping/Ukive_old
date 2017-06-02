#pragma once
#include "TextSpan.h"

class UnderlineSpan : public TextSpan
{
public:
	UnderlineSpan(unsigned int start, unsigned int end);
	virtual ~UnderlineSpan();

	virtual int getType();
};

