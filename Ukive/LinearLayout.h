﻿#pragma once
#include "ViewGroup.h"

class LayoutParams;

class LinearLayout : public ViewGroup
{
public:
	static const int VERTICAL = 1;
	static const int HORIZONTAL = 2;

protected:
	virtual LayoutParams *generateLayoutParams(
		LayoutParams *lp) override;
	virtual LayoutParams *generateDefaultLayoutParams() override;
	virtual bool checkLayoutParams(LayoutParams *lp) override;

private:
	int mOrientation;

	void measureWeightedChildren(
		int totalWeight,
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);
	void measureSequenceChildren(
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);
	void measureLinearLayoutChildren(
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);

	void measureVertical(int width, int height, int widthSpec, int heightSpec);
	void measureHorizontal(int width, int height, int widthSpec, int heightSpec);

	void layoutVertical(int left, int top, int right, int bottom);
	void layoutHorizontal(int left, int top, int right, int bottom);

public:
	LinearLayout(UWindow *wnd);
	LinearLayout(UWindow *wnd, int id);
	~LinearLayout();

	void setOrientation(int orientation);

	virtual void onMeasure(
		int width, int height, 
		int widthSpec, int heightSpec) override;
	virtual void onLayout(
		bool changed, bool sizeChanged, 
		int left, int top, int right, int bottom) override;
};