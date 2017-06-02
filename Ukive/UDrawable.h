#pragma once

class UCanvas;
class UWindow;

class UDrawable
{
private:
	int mState;
	int mPrevState;
	bool mParentHasFocus;
	D2D1_RECT_F mRect;

protected:
	int mStartX, mStartY;
	UWindow *mOwnerWindow;

	virtual bool onFocusChanged(bool focus);
	virtual void onBoundChanged(D2D1_RECT_F &newBound);
	virtual bool onStateChanged(int newState, int prevState);
	
public:
	UDrawable(UWindow *wnd);
	virtual ~UDrawable();

	const static int STATE_NONE = 0;
	const static int STATE_PRESSED = 1;
	const static int STATE_FOCUSED = 2;
	const static int STATE_HOVERED = 3;

	void setBound(D2D1_RECT_F &rect);
	void setBound(int left, int top, int right, int bottom);

	bool setState(int state);
	void setHotspot(int x, int y);
	bool setParentFocus(bool focus);

	int getState();
	int getPrevState();
	D2D1_RECT_F &getBound();

	virtual void draw(UCanvas *canvas) = 0;

	virtual float getOpacity();

	virtual int getIncWidth();
	virtual int getIncHeight();
};