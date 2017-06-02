#include "UCommon.h"
#include "InlineObjectSpan.h"


InlineObjectSpan::InlineObjectSpan(unsigned int start, unsigned int end)
	:USpan(start, end)
{
}

InlineObjectSpan::~InlineObjectSpan()
{
}


int InlineObjectSpan::getBaseType()
{
	return USpan::USPAN_BASE_TYPE_INLINEOBJECT;
}