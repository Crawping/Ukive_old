#pragma once
#include "UWidgetGroup.h"

class ULayoutParams;

class ULinearLayout : public UWidgetGroup
{
public:
	static const int VERTICAL = 1;
	static const int HORIZONTAL = 2;

protected:
	virtual ULayoutParams *generateLayoutParams(
		ULayoutParams *lp) override;
	virtual ULayoutParams *generateDefaultLayoutParams() override;
	virtual bool checkLayoutParams(ULayoutParams *lp) override;

private:
	int mOrientation;

	void measureWeightedChildren(
		int totalWeight,
		int parentWidth, int parentHeight,
		int parentWidthSpec, int parentHeightSpec);
	void measureLinearLayoutChildren(
		int parentWidth, int parentHeight,
		int parentWidthSpec, int parentHeightSpec);

	void measureVertical(int width, int height, int widthSpec, int heightSpec);
	void measureHorizontal(int width, int height, int widthSpec, int heightSpec);

	void layoutVertical(int left, int top, int right, int bottom);
	void layoutHorizontal(int left, int top, int right, int bottom);

public:
	ULinearLayout(UWindow *wnd);
	ULinearLayout(UWindow *wnd, int id);
	~ULinearLayout();

	void setOrientation(int orientation);

	virtual void onMeasure(
		int width, int height, 
		int widthSpec, int heightSpec) override;
	virtual void onLayout(
		bool changed, bool sizeChanged, 
		int left, int top, int right, int bottom) override;
};