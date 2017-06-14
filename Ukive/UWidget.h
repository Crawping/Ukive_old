#pragma once
#include "UExecutable.h"

class UWindow;
class UCanvas;
class UDrawable;
class UInputEvent;
class UInputConnection;
class ULayoutParams;
class OnClickListener;
class UWidgetAnimator;

enum UGravity
{
	LEFT,
	TOP,
	RIGHT,
	BOTTOM,
	CENTER,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,
};

class UWidget
{
public:
	const static int FIT = 1;
	const static int EXACTLY = 2;
	const static int UNKNOWN = 3;

	const static int VISIBLE = 0;
	const static int INVISIBLE = 1;
	const static int VANISHED = 2;

private:
	//普通动画变量。
	double mAlpha;
	double mScaleX;
	double mScaleY;
	double mTranslateX;
	double mTranslateY;
	double mPivotX, mPivotY;

	//揭露动画变量。
	int mRevealType;
	bool mHasReveal;
	double mRevealRadius;
	double mRevealCenterX;
	double mRevealCenterY;
	double mRevealWidthRadius;
	double mRevealHeightRadius;

	UWidget *mParent;
	ULayoutParams *mLayoutParams;
	UInputConnection *mICHolder;
	OnClickListener *mClickListener;
	UWidgetAnimator *mAnimator;

	class ClickPerformer : public UExecutable
	{
	private:
		UWidget *mWidget;
	public:
		ClickPerformer(UWidget *w)
		{
			mWidget = w;
		}
		void run() override
		{
			mWidget->performClick();
		}
	};

	ClickPerformer *mClickPerformer;

protected:
	int mId;
	
	int mLeft;
	int mTop;
	int mRight;
	int mBottom;

	int mScrollX;
	int mScrollY;

	int mPaddingLeft;
	int mPaddingTop;
	int mPaddingRight;
	int mPaddingBottom;

	int mMeasuredWidth;
	int mMeasuredHeight;

	int mMinimumWidth;
	int mMinimumHeight;

	int mVisibility;
	float mElevation;
	
	bool mIsEnable;
	bool mIsAttachdToWindow;
	bool mIsInputEventAtLast;
	bool mIsPressed;
	bool mHasFocus;
	bool mIsFocusable;
	bool mIsLayouted;
	bool mIsReceiveOutsideInputEvent;
	bool mCanConsumeMouseEvent;

	UWindow *mWindow;
	UDrawable *mBackgroundDrawable;
	UDrawable *mForegroundDrawable;

	virtual void dispatchDraw(UCanvas *canvas);
	virtual void dispatchDiscardFocus();
	virtual void dispatchDiscardPendingOperations();

	void drawBackgroundWithShadow(UCanvas *canvas);
	void drawBackground(UCanvas *canvas);
	void drawForeground(UCanvas *canvas);

	void setMeasuredDimension(int width, int height);

	void performClick();

public:
	UWidget(UWindow *wnd);
	UWidget(UWindow *wnd, int id);
	virtual ~UWidget();

	UWidgetAnimator *animate();

	void setX(double x);
	void setY(double y);
	void setAlpha(double alpha);
	void setScaleX(double sx);
	void setScaleY(double sy);
	void setTranslateX(double tx);
	void setTranslateY(double ty);
	void setPivotX(double px);
	void setPivotY(double py);

	void setRevealType(int type);
	void setHasReveal(bool reveal);
	void setRevealRadius(double radius);
	void setRevealCenterX(double cx);
	void setRevealCenterY(double cy);
	void setRevealWidthRadius(double widthRadius);
	void setRevealHeightRadius(double heightRadius);

	void setScrollX(int x);
	void setScrollY(int y);
	void setVisibility(int visibility);
	void setEnabled(bool enable);
	void setBackground(UDrawable *drawable);
	void setForeground(UDrawable *drawable);
	void setPadding(int left, int top, int right, int bottom);
	void setLayoutParams(ULayoutParams *params);
	void setIsInputEventAtLast(bool isInput);
	void setPressed(bool pressed);
	void setCurrentCursor(LPCWSTR cursor);
	void setFocusable(bool focusable);
	void setElevation(float elevation);
	void setReceiveOutsideInputEvent(bool receive);
	void setCanConsumeMouseEvent(bool enable);

	void setMinimumWidth(int width);
	void setMinimumHeight(int height);

	void setOnClickListener(OnClickListener *l);

	//设置该Widget的父Widget，该方法由框架调用。
	void setParent(UWidget *parent);

	double getX();
	double getY();
	double getAlpha();
	double getScaleX();
	double getScaleY();
	double getTranslateX();
	double getTranslateY();
	double getPivotX();
	double getPivotY();

	int getId();
	int getScrollX();
	int getScrollY();
	int getLeft();
	int getTop();
	int getRight();
	int getBottom();
	int getWidth();
	int getHeight();
	int getMeasuredWidth();
	int getMeasuredHeight();
	float getElevation();
	int getVisibility();

	int getMinimumWidth();
	int getMinimumHeight();

	int getPaddingLeft();
	int getPaddingTop();
	int getPaddingRight();
	int getPaddingBottom();

	ULayoutParams *getLayoutParams();
	UWidget *getParent();

	UWindow *getWindow();
	UDrawable *getBackground();
	UDrawable *getForeground();

	URect getBound();
	URect getBoundInWindow();
	URect getBoundInScreen();

	virtual UWidget *findWidgetById(int id);
	
	bool isEnabled();
	bool isAttachedToWindow();
	bool isInputEventAtLast();
	bool isPressed();
	bool hasFocus();
	bool isFocusable();
	bool isLayouted();
	bool isMouseInThis(UInputEvent *e);
	bool isGroupMouseInThis(UInputEvent *e);
	bool isReceiveOutsideInputEvent();
	bool canConsumeMouseEvent();

	void scrollTo(int x, int y);
	void scrollBy(int dx, int dy);

	void draw(UCanvas *canvas);
	void measure(int width, int height, int widthMode, int heightMode);
	void layout(int left, int top, int right, int bottom);

	void invalidate();
	void invalidate(URect rect);
	void invalidate(int left, int top, int right, int bottom);
	void requestLayout();

	void requestFocus();
	void discardFocus();
	void discardPendingOperations();

	virtual bool dispatchInputEvent(UInputEvent *e);
	virtual void dispatchWindowFocusChanged(bool windowFocus);

	virtual void onAttachedToWindow();
	virtual void onDetachedFromWindow();

protected:
	virtual void onDraw(UCanvas *canvas);
	virtual void onMeasure(
		int width, int height, 
		int widthMode, int heightMode);
	virtual void onLayout(
		bool changed, bool sizeChanged, 
		int left, int top, int right, int bottom);
	virtual bool onInputEvent(UInputEvent *e);
	
	virtual bool onCheckIsTextEditor();
	virtual UInputConnection *onCreateInputConnection();

	virtual void onSizeChanged(int width, int height, int oldWidth, int oldHeight);
	virtual void onVisibilityChanged(int visibility);
	virtual void onFocusChanged(bool getFocus);
	virtual void onWindowFocusChanged(bool windowFocus);
	virtual void onScrollChanged(int scrollX, int scrollY, int oldScrollX, int oldScrollY);
};