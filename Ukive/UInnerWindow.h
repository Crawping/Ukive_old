#pragma once
#include "UFrameLayout.h"

enum Gravity;
class UCanvas;
class UWindow;
class UWidget;
class UInputEvent;
class UDrawable;

class UInnerWindow
{
public:
	class InnerDecorView : public UFrameLayout
	{
	private:
		UInnerWindow *mInnerWindow;

	public:
		InnerDecorView(UInnerWindow *inner);
		InnerDecorView(UInnerWindow *inner, int id);
		~InnerDecorView();

		virtual bool onInterceptInputEvent(UInputEvent *e) override;
		virtual bool onInputEvent(UInputEvent *e) override;
	};

private:
	int mWidth;
	int mHeight;
	float mElevation;
	bool mOutsideTouchable;
	bool mDismissByTouchOutside;
	UDrawable *mBackgroundDrawable;

	UWindow *mParent;
	UWidget *mContentView;
	InnerDecorView *mDecorView;
	bool mIsShowing;

	void createDecorView();

public:
	UInnerWindow(UWindow *wnd);
	virtual ~UInnerWindow();

	void setWidth(int width);
	void setHeight(int height);
	void setSize(int width, int height);
	void setElevation(float elevation);
	void setBackground(UDrawable *drawable);
	void setOutsideTouchable(bool touchable);
	void setDismissByTouchOutside(bool enable);
	void setContentView(UWidget *contentView);

	int getWidth();
	int getHeight();
	float getElevation();
	UDrawable *getBackground();
	bool isOutsideTouchable();
	bool isDismissByTouchOutside();
	UWindow *getParent();
	UWidget *getContentView();
	UWidget *getDecorView();

	bool isShowing();
	
	void show(int x, int y);
	void show(UWidget *anchor, Gravity gravity);
	void update(int x, int y);
	void update(UWidget *anchor, Gravity gravity);
	void dismiss();
};