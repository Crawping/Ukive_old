#include "UCommon.h"
#include "TextSpan.h"


TextSpan::TextSpan(unsigned int start, unsigned int end)
	:USpan(start, end)
{
}

TextSpan::~TextSpan()
{
}


int TextSpan::getBaseType()
{
	return USpan::USPAN_BASE_TYPE_TEXT;
}