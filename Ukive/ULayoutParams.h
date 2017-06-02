#pragma once

class ULayoutParams
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
	ULayoutParams(int width, int height);
	ULayoutParams(ULayoutParams *lp);
	virtual ~ULayoutParams();
};