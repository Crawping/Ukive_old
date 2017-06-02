#pragma once
#include "UWidget.h"

class UWidgetGroup : public UWidget
{
private:
	std::vector<std::shared_ptr<UWidget>> mWidgetList;

	void initViewGroup();

protected:
	virtual void dispatchDraw(UCanvas *canvas) override;
	virtual void dispatchDiscardFocus() override;
	virtual void dispatchDiscardPendingOperations() override;

	virtual bool dispatchMouseEvent(UInputEvent *e);
	virtual bool dispatchKeyboardEvent(UInputEvent *e);

	virtual ULayoutParams *generateLayoutParams(ULayoutParams *lp);
	virtual ULayoutParams *generateDefaultLayoutParams();
	virtual bool checkLayoutParams(ULayoutParams *lp);

	int getWrappedWidth();
	int getWrappedHeight();

public:
	UWidgetGroup(UWindow *wnd);
	UWidgetGroup(UWindow *wnd, int id);
	virtual ~UWidgetGroup();

	virtual bool dispatchInputEvent(UInputEvent *e) override;
	virtual void dispatchWindowFocusChanged(bool windowFocus) override;

	virtual void onLayout(
		bool changed, bool sizeChanged, 
		int left, int top, int right, int bottom) = 0;

	virtual bool onInterceptInputEvent(UInputEvent *e);
	virtual bool onInterceptMouseEvent(UInputEvent *e);
	virtual bool onInterceptKeyboardEvent(UInputEvent *e);

	virtual void onAttachedToWindow() override;
	virtual void onDetachedFromWindow() override;

	void addWidget(UWidget *widget);
	void addWidget(UWidget *widget, ULayoutParams *params);
	void addWidget(std::size_t index, UWidget *widget, ULayoutParams *params);
	void removeWidget(UWidget *widget);
	void removeWidget(std::size_t index);

	virtual UWidget *findWidgetById(int id) override;

	static void getChildMeasure(
		int parentSize, int parentSizeMode,
		int padding, int childDimension,
		int *childSize, int *childSizeMode);

	std::size_t getChildCount();
	UWidget *getChildById(int id);
	UWidget *getChildAt(std::size_t index);

	void drawChild(UCanvas *canvas, UWidget *child);
	void drawChildren(UCanvas *canvas);

	void measureChild(
		UWidget *child, 
		int parentWidth, int parentHeight, 
		int parentWidthMode, int parentHeightMode);

	void measureChildWithMargins(
		UWidget *child,
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);

	void measureChildren(
		int parentWidth, int parentHeight, 
		int parentWidthMode, int parentHeightMode);

	void measureChildrenWithMargins(
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);
};