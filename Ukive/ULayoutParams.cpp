#include "ULayoutParams.h"


ULayoutParams::ULayoutParams(int width, int height)
{
	this->width = width;
	this->height = height;
	leftMargin = topMargin = rightMargin = bottomMargin = 0;
}

ULayoutParams::ULayoutParams(ULayoutParams *lp)
{
	this->width = lp->width;
	this->height = lp->height;

	leftMargin = lp->leftMargin;
	rightMargin = lp->rightMargin;
	topMargin = lp->topMargin;
	bottomMargin = lp->bottomMargin;
}

ULayoutParams::~ULayoutParams()
{
}