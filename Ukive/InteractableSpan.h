#pragma once
#include "USpan.h"

class InteractableSpan : public USpan
{
public:
	InteractableSpan(unsigned int start, unsigned int end);
	virtual ~InteractableSpan();

	int getBaseType();
};