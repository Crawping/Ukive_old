#pragma once
#include "UMenu.h"
#include "UCycler.h"
#include "UExecutable.h"
#include "UAnimationManager.h"
#include "UMenuCallback.h"

enum Gravity;

class UCanvas;
class UWidget;
class UMessage;
class UInputEvent;
class UBaseLayout;
class UApplication;
class UWindowManager;
class UInputConnection;
class URenderer;
class UBitmapFactory;
class TextActionMode;
class TextActionModeCallback;
class UMenuCallback;

class UWindow
{
private:
	HWND mWindowHandle;
	HCURSOR mCursor;

	UApplication *mApplication;
	UAnimationManager *mAnimationManager;
	UAnimationManager::OnStateChangedListener *mStateChangedListener;

	UBaseLayout *mBaseLayout;
	TextActionMode *mTextActionMode;

	UCanvas *mCanvas;
	UBitmapFactory *mBitmapFactory;
	URenderer *mRenderer;

	static const int SCHEDULE_RENDER = 0;
	static const int SCHEDULE_LAYOUT = 1;

	class UpdateCycler : public UCycler
	{
	private:
		UWindow *mWindow;
	public:
		UpdateCycler(UWindow *window);

		void handleMessage(UMessage *msg);
	};

	class AnimStateChangedListener 
		: public UAnimationManager::OnStateChangedListener
	{
	private:
		UWindow *mWindow;
	public:
		AnimStateChangedListener(UWindow *window);

		void onStateChanged(
			UI_ANIMATION_MANAGER_STATUS newStatus,
			UI_ANIMATION_MANAGER_STATUS previousStatus);
	};

	class AnimTimerEventListener
		: public UAnimationManager::OnTimerEventListener
	{
	private:
		UWindow *mWindow;
	public:
		AnimTimerEventListener(UWindow *window);

		void OnPreUpdate();
		void OnPostUpdate();
		void OnRenderingTooSlow(unsigned int fps);
	};

	UCycler *mLabourCycler;
	AnimStateChangedListener *mAnimStateChangedListener;
	AnimTimerEventListener *mAnimTimerEventListener;

protected:
	bool mIsCreated;
	bool mIsShowed;
	bool mIsEnableMouseTrack;

	int mX, mY;
	int mPrevX, mPrevY;
	int mWidth, mHeight;
	int mMinWidth, mMinHeight;
	int mPrevWidth, mPrevHeight;
	int mClientWidth, mClientHeight;
	int mPrevClientWidth, mPrevClientHeight;
	D2D1_COLOR_F mBackgroundColor;

	int mId;
	std::wstring mTitle;

	UWidget *mMouseHolder;
	UWidget *mFocusHolder;
	UWidget *mFocusHolderBackup;
	unsigned int mMouseHolderRef;

	void setMouseTrack();

	virtual bool onRequestCreate();
	virtual bool onRequestResize(
		int newWidth, int newHeight, 
		int oldWidth, int oldHeight);
	virtual bool onRequestMove(int x, int y, int dx, int dy);
	virtual bool onRequestShow(bool show);
	virtual bool onRequestFocus();
	virtual bool onRequestDraw(URect rect);
	virtual bool onRequestClose();
	
public:
	UWindow(UApplication *app);
	UWindow(UApplication *app, int id);
	virtual ~UWindow();

	static const UINT MSG_INVALIDATE = 0;
	static const UINT MSG_RELAYOUT = 1;

	UWidget *findWidgetById(int id);

	HWND getWindowHandle();
	UApplication *getApplication();
	UAnimationManager *getAnimationManager();
	UCycler *getCycler();

	URenderer *getRenderer();
	UBitmapFactory *getBitmapFactory();

	void setTitle(std::wstring title);
	void setX(int x);
	void setY(int y);
	void setPosition(int x, int y);
	void setWidth(int width);
	void setHeight(int height);
	void setMinWidth(int minWidth);
	void setMinHeight(int minHeight);
	void setSize(int width, int height);
	void setAsStartupWindow();
	void setCurrentCursor(LPCWSTR cursor);
	void setContentView(UWidget *content);
	void setBackgroundColor(D2D1_COLOR_F color);

	int getId();
	std::wstring getTitle();
	int getX();
	int getY();
	void getPosition(UPoint *point);
	int getWidth();
	int getHeight();
	int getMinWidth();
	int getMinHeight();
	int getClientWidth();
	int getClientHeight();
	HCURSOR getCurrentCursor();
	D2D1_COLOR_F getBackgroundColor();
	UBaseLayout *getBaseLayout();

	void captureMouse(UWidget *widget);
	void releaseMouse();
	UWidget *getMouseHolder();
	unsigned int getMouseHolderRef();

	//当一个widget获取到焦点时，应调用此方法。
	void captureKeyboard(UWidget *widget);

	//当一个widget放弃焦点时，应调用此方法。
	void releaseKeyboard();
	UWidget *getKeyboardHolder();

	float calculateDip(float value);

	TextActionMode *startTextActionMode(TextActionModeCallback *callback);

	bool isCreated();
	bool isShowed();
	bool isStartupWindow();
	bool isCursorInClient();

	void show();
	void hide();
	void focus();
	void close();
	void close(bool notify);

	void notifyWindowSizeChanged(
		int param, int width, int height, 
		int clientWidth, int clientHeight);
	void notifyWindowLocationChanged(int x, int y);

	void invalidate();
	void invalidate(int left, int top, int right, int bottom);
	void requestLayout();

	void performLayout();
	void performRefresh();
	void performRefresh(int left, int top, int right, int bottom);

	virtual void onCreate();
	virtual void onShow(bool show);
	virtual void onActivate(int param);
	virtual void onDraw(UCanvas *canvas);
	virtual void onMove(int x, int y);
	virtual void onResize(
		int param, int width, int height, 
		int clientWidth, int clientHeight);
	virtual bool onMoving(RECT *rect);
	virtual bool onResizing(WPARAM edge, RECT *rect);
	virtual bool onClose();
	virtual void onDestroy();
	virtual void onInputEvent(UInputEvent *e);
};