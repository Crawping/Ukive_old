#pragma once
#include "UWidgetGroup.h"

class RestrainLayoutParams;

class RestrainLayout : public UWidgetGroup
{
public:
	RestrainLayout(UWindow *wnd);
	RestrainLayout(UWindow *wnd, int id);
	~RestrainLayout();

protected:
	virtual ULayoutParams *generateLayoutParams(ULayoutParams *lp) override;
	virtual ULayoutParams *generateDefaultLayoutParams() override;
	virtual bool checkLayoutParams(ULayoutParams *lp) override;

private:
	UWidget *getChildById(int id);
	void clearMeasureFlag();

	void measureRestrainedChildren(
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);

	void measureRestrainedChild(
		UWidget *child, RestrainLayoutParams *lp,
		int parentWidth, int parentHeight, int parentWidthMode, int parentHeightMode);

	void getRestrainedChildWidth(
		UWidget *child, RestrainLayoutParams *lp,
		int parentWidth, int parentWidthMode, int *width, int *widthMode);

	void getRestrainedChildHeight(
		UWidget *child, RestrainLayoutParams *lp,
		int parentHeight, int parentHeightMode, int *height, int *heightMode);

	int getLeftSpacing(UWidget *child, RestrainLayoutParams *lp);
	int getTopSpacing(UWidget *child, RestrainLayoutParams *lp);
	int getRightSpacing(UWidget *child, RestrainLayoutParams *lp);
	int getBottomSpacing(UWidget *child, RestrainLayoutParams *lp);

	int measureWrappedWidth();
	int measureWrappedHeight();

	void layoutChild(
		UWidget *child, RestrainLayoutParams *lp,
		int left, int top, int right, int bottom);

	void layoutChildVertical(
		UWidget *child, RestrainLayoutParams *lp,
		int top, int bottom);
	void layoutChildHorizontal(
		UWidget *child, RestrainLayoutParams *lp,
		int left, int right);

public:
	virtual void onMeasure(
		int width, int height,
		int widthMode, int heightMode) override;
	virtual void onLayout(
		bool changed, bool sizeChanged,
		int left, int top, int right, int bottom) override;
};