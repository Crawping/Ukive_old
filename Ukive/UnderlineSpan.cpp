#include "UCommon.h"
#include "UnderlineSpan.h"


UnderlineSpan::UnderlineSpan(unsigned int start, unsigned int end)
	:TextSpan(start, end)
{
}


UnderlineSpan::~UnderlineSpan()
{
}


int UnderlineSpan::getType()
{
	return USpan::TEXT_UNDERLINE;
}