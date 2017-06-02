#include "UCommon.h"
#include "UInputEvent.h"
#include "InteractableSpan.h"


InteractableSpan::InteractableSpan(
	unsigned int start, unsigned int end)
	:USpan(start, end)
{
}

InteractableSpan::~InteractableSpan()
{
}


int InteractableSpan::getBaseType()
{
	return USpan::USPAN_BASE_TYPE_INTERACTABLE;
}