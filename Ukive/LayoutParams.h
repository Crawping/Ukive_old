#pragma once

class LayoutParams
{
public:
	int width;
	int height;

	int leftMargin;
	int topMargin;
	int rightMargin;
	int bottomMargin;

	const static int FIT_CONTENT = -1;
	const static int MATCH_PARENT = -2;

public:
	LayoutParams(int width, int height);
	LayoutParams(LayoutParams *lp);
	virtual ~LayoutParams();
};