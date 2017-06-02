#include "RestrainLayoutParams.h"



RestrainLayoutParams::RestrainLayoutParams(int width, int height)
	: ULayoutParams(width, height)
{
	initLayoutParams();
}

RestrainLayoutParams::RestrainLayoutParams(ULayoutParams *lp)
	: ULayoutParams(lp)
{
	initLayoutParams();
}


RestrainLayoutParams::~RestrainLayoutParams()
{
}


void RestrainLayoutParams::initLayoutParams()
{
	isWidthMeasured = false;
	isHeightMeasured = false;

	isVertLayouted = false;
	isHoriLayouted = false;

	startHandledEdge = EDGE_NONE;
	topHandledEdge = EDGE_NONE;
	endHandledEdge = EDGE_NONE;
	bottomHandledEdge = EDGE_NONE;

	verticalPercent = .5f;
	horizontalPercent = .5f;
}


bool RestrainLayoutParams::hasStart()
{
	return startHandledEdge != EDGE_NONE;
}

bool RestrainLayoutParams::hasTop()
{
	return topHandledEdge != EDGE_NONE;
}

bool RestrainLayoutParams::hasEnd()
{
	return endHandledEdge != EDGE_NONE;
}

bool RestrainLayoutParams::hasBottom()
{
	return bottomHandledEdge != EDGE_NONE;
}

bool RestrainLayoutParams::hasVerticalCouple()
{
	return (topHandledEdge != EDGE_NONE
		&& bottomHandledEdge != EDGE_NONE);
}

bool RestrainLayoutParams::hasHorizontalCouple()
{
	return (startHandledEdge != EDGE_NONE
		&& endHandledEdge != EDGE_NONE);
}