#pragma once
#include "View.h"

class ViewGroup : public View
{
private:
	std::vector<std::shared_ptr<View>> mWidgetList;

	void initViewGroup();

protected:
	virtual void dispatchDraw(UCanvas *canvas) override;
	virtual void dispatchDiscardFocus() override;
	virtual void dispatchDiscardPendingOperations() override;

	virtual bool dispatchMouseEvent(UInputEvent *e);
	virtual bool dispatchKeyboardEvent(UInputEvent *e);

	virtual LayoutParams *generateLayoutParams(LayoutParams *lp);
	virtual LayoutParams *generateDefaultLayoutParams();
	virtual bool checkLayoutParams(LayoutParams *lp);

	int getWrappedWidth();
	int getWrappedHeight();

public:
	ViewGroup(UWindow *wnd);
	ViewGroup(UWindow *wnd, int id);
	virtual ~ViewGroup();

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

	void addWidget(View *widget);
	void addWidget(View *widget, LayoutParams *params);
	void addWidget(std::size_t index, View *widget, LayoutParams *params);
	void addWidget(std::shared_ptr<View> widget);
	void addWidget(std::shared_ptr<View> widget, LayoutParams *params);
	void addWidget(std::size_t index, std::shared_ptr<View> widget, LayoutParams *params);
	void removeWidget(View *widget);
	void removeWidget(std::shared_ptr<View> widget);
	void removeWidget(std::size_t index);

	virtual View *findWidgetById(int id) override;

	static void getChildMeasure(
		int parentSize, int parentSizeMode,
		int padding, int childDimension,
		int *childSize, int *childSizeMode);

	std::size_t getChildCount();
	View *getChildById(int id);
	View *getChildAt(std::size_t index);

	void drawChild(UCanvas *canvas, View *child);
	void drawChildren(UCanvas *canvas);

	void measureChild(
		View *child, 
		int parentWidth, int parentHeight, 
		int parentWidthMode, int parentHeightMode);

	void measureChildWithMargins(
		View *child,
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);

	void measureChildren(
		int parentWidth, int parentHeight, 
		int parentWidthMode, int parentHeightMode);

	void measureChildrenWithMargins(
		int parentWidth, int parentHeight,
		int parentWidthMode, int parentHeightMode);
};