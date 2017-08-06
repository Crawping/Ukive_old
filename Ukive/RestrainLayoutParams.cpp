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


RestrainLayoutParams* RestrainLayoutParams::startHandle(int handleId, int handleEdge, int margin)
{
	startHandledId = handleId;
	startHandledEdge = handleEdge;
	leftMargin = margin;
	return this;
}

RestrainLayoutParams* RestrainLayoutParams::topHandle(int handleId, int handleEdge, int margin)
{
	topHandledId = handleId;
	topHandledEdge = handleEdge;
	topMargin = margin;
	return this;
}

RestrainLayoutParams* RestrainLayoutParams::endHandle(int handleId, int handleEdge, int margin)
{
	endHandledId = handleId;
	endHandledEdge = handleEdge;
	rightMargin = margin;
	return this;
}

RestrainLayoutParams* RestrainLayoutParams::bottomHandle(int handleId, int handleEdge, int margin)
{
	bottomHandledId = handleId;
	bottomHandledEdge = handleEdge;
	bottomMargin = margin;
	return this;
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