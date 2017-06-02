#include "UCommon.h"
#include "USpan.h"


USpan::USpan(unsigned int start, unsigned int end)
{
	mStart = start;
	mEnd = end;
}

USpan::~USpan()
{
}


void USpan::resize(unsigned int start, unsigned int end)
{
	mStart = start;
	mEnd = end;
}

unsigned int USpan::getStart()
{
	return mStart;
}

unsigned int USpan::getEnd()
{
	return mEnd;
}