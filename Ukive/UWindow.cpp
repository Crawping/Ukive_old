#include "UCommon.h"
#include "UTags.h"
#include "UColor.h"
#include "UCanvas.h"
#include "UApplication.h"
#include "UWindowManager.h"
#include "UWindowClass.h"
#include "UDeviceManager.h"
#include "UInputEvent.h"
#include "BaseLayout.h"
#include "BaseLayoutParams.h"
#include "LayoutParams.h"
#include "UInputConnection.h"
#include "UTsfManager.h"
#include "View.h"
#include "UMessage.h"
#include "UBitmapFactory.h"
#include "UAnimationManager.h"
#include "URenderer.h"
#include "TextActionMode.h"
#include "TextActionModeCallback.h"
#include "UContextMenu.h"
#include "UContextMenuCallback.h"
#include "UColorDrawable.h"
#include "UWindowSlave.h"
#include "UWindow.h"


UWindow::UWindow(UApplication *app)
	:UWindow(app, UApplication::getWindowUID())
{
}

UWindow::UWindow(UApplication *app, int id)
	:mBackgroundColor(D2D1::ColorF(D2D1::ColorF::White))
{
	if (app == nullptr)
		throw std::invalid_argument(
			"UWindow-Constructor(): null UApplication* param.");
	if (app->getWindowManager()->isContain(id))
		throw std::invalid_argument("UWindow-Constructor(): id duplicated.");

	mApplication = app;
	mWindowHandle = nullptr;
	mCursor = ::LoadCursorW(nullptr, IDC_ARROW);

	mIsCreated = false;
	mIsShowing = false;

	mPrevX = mX = 0;
	mPrevY = mY = 0;
	mWidth = 200;
	mHeight = 100;
	mMinWidth = mMinHeight = 0;
	mPrevWidth = mPrevHeight = -1;
	mPrevClientWidth = mClientWidth = -1;
	mPrevClientHeight = mClientHeight = -1;

	mId = id;
	mTitle = L"UWindow";
	mMouseHolder = nullptr;
	mFocusHolder = nullptr;
	mFocusHolderBackup = nullptr;
	mMouseHolderRef = 0;

	mIsEnableMouseTrack = true;

	mLabourCycler = nullptr;
	mBaseLayout = nullptr;
	mAnimationManager = nullptr;
	mAnimStateChangedListener = nullptr;
	mAnimTimerEventListener = nullptr;

	mCanvas = nullptr;
	mRenderer = nullptr;
	mBitmapFactory = nullptr;
	mTextActionMode = nullptr;
	mContextMenu = nullptr;
	mSlave = nullptr;
}


UWindow::~UWindow()
{
}


View *UWindow::findWidgetById(int id)
{
	return mBaseLayout->findWidgetById(id);
}

HWND UWindow::getWindowHandle()
{
	return mWindowHandle;
}

UApplication *UWindow::getApplication()
{
	return mApplication;
}

UAnimationManager *UWindow::getAnimationManager()
{
	return mAnimationManager;
}

UCycler *UWindow::getCycler()
{
	return mLabourCycler;
}

URenderer *UWindow::getRenderer()
{
	return mRenderer;
}

UBitmapFactory *UWindow::getBitmapFactory()
{
	return mBitmapFactory;
}


void UWindow::setMouseTrack()
{
	if (mIsEnableMouseTrack)
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE | TME_HOVER;
		csTME.hwndTrack = mWindowHandle;// 指定要 追踪 的窗口
		csTME.dwHoverTime = 1000;  // 鼠标在按钮上停留超过 1s ，才认为状态为 HOVER
		::_TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持

		mIsEnableMouseTrack = false;
	}
}


void UWindow::setHandleForCreate(HWND hWnd)
{
	mWindowHandle = hWnd;

	RECT clientRect;
	::GetClientRect(mWindowHandle, &clientRect);
	mClientWidth = clientRect.right - clientRect.left;
	mClientHeight = clientRect.bottom - clientRect.top;
}


void UWindow::setTitle(std::wstring title)
{
	if (mTitle == title)
		return;

	mTitle = title;

	::SetWindowTextW(this->mWindowHandle, mTitle.c_str());
}

void UWindow::setX(int x)
{
	if (x == mX)
		return;

	if (!this->onRequestMove(x, mY, x - mX, 0))
		mX = x;
}

void UWindow::setY(int y)
{
	if (y == mY)
		return;

	if (!this->onRequestMove(mX, y, 0, y - mY))
		mY = y;
}

void UWindow::setPosition(int x, int y)
{
	if (x == mX && y == mY)
		return;

	if (!this->onRequestMove(mX, mY, x - mX, y - mY))
	{
		mX = x;
		mY = y;
	}
}

void UWindow::setWidth(int width)
{
	if (width == mWidth)
		return;

	if (!this->onRequestResize(width, mHeight, mWidth, mHeight))
		mWidth = width;
}

void UWindow::setHeight(int height)
{
	if (height == mHeight)
		return;

	if (!this->onRequestResize(mWidth, height, mWidth, mHeight))
		mHeight = height;
}

void UWindow::setMinWidth(int minWidth)
{
	mMinWidth = minWidth;
}

void UWindow::setMinHeight(int minHeight)
{
	mMinHeight = minHeight;
}

void UWindow::setSize(int width, int height)
{
	if (width == mWidth && height == mHeight)
		return;

	if (!this->onRequestResize(width, height, mWidth, mHeight))
	{
		mWidth = width;
		mHeight = height;
	}
}

void UWindow::setAsStartupWindow()
{
	mApplication->getWindowManager()->setStartupWindow(mId);
}

void UWindow::setCurrentCursor(LPCWSTR cursor)
{
	mCursor = ::LoadCursorW(0, cursor);
	::SetCursor(mCursor);
}

void UWindow::setContentView(View *content)
{
	mBaseLayout->addContent(content);
}

void UWindow::setBackgroundColor(D2D1_COLOR_F color)
{
	mBackgroundColor = color;
	this->invalidate();
}


int UWindow::getId()
{
	return mId;
}

std::wstring UWindow::getTitle()
{
	return mTitle;
}

int UWindow::getX()
{
	return mX;
}

int UWindow::getY()
{
	return mY;
}

void UWindow::getPosition(UPoint *point)
{
	if (point == nullptr)
		throw std::invalid_argument("UWindow-getPosition(): null pointer.");

	point->x = mX;
	point->y = mY;
}

int UWindow::getWidth()
{
	return mWidth;
}

int UWindow::getHeight()
{
	return mHeight;
}

int UWindow::getMinWidth()
{
	return mMinWidth;
}

int UWindow::getMinHeight()
{
	return mMinHeight;
}

int UWindow::getClientWidth()
{
	return mClientWidth;
}

int UWindow::getClientHeight()
{
	return mClientHeight;
}

HCURSOR UWindow::getCurrentCursor()
{
	return mCursor;
}

D2D1_COLOR_F UWindow::getBackgroundColor()
{
	return mBackgroundColor;
}

BaseLayout *UWindow::getBaseLayout()
{
	return mBaseLayout;
}


void UWindow::captureMouse(View *widget)
{
	if (widget == nullptr)
		return;

	//当已存在有捕获鼠标的Widget时，若此次调用该方法的Widget
	//与之前不同，此次调用将被忽略。在使用中应避免此种情况产生。
	if (mMouseHolderRef != 0
		&& widget != mMouseHolder)
	{
		::OutputDebugString(L"abnormal capture mouse!!\n");
		return;
	}

	++mMouseHolderRef;
	::OutputDebugString(L"capture mouse!!\n");

	//该Widget第一次捕获鼠标。
	if (mMouseHolderRef == 1)
	{
		::SetCapture(mWindowHandle);
		mMouseHolder = widget;
	}
}

void UWindow::releaseMouse()
{
	if (mMouseHolderRef == 0)
		return;

	--mMouseHolderRef;
	::OutputDebugString(L"release mouse!!\n");

	//鼠标将被释放。
	if (mMouseHolderRef == 0)
	{
		::ReleaseCapture();
		mMouseHolder = 0;
	}
}

View *UWindow::getMouseHolder()
{
	return mMouseHolder;
}

unsigned int UWindow::getMouseHolderRef()
{
	return mMouseHolderRef;
}


void UWindow::captureKeyboard(View *widget)
{
	mFocusHolder = widget;
	//::OutputDebugString(L"captureKeyboard!!\n");
}

void UWindow::releaseKeyboard()
{
	mFocusHolder = 0;
	//::OutputDebugString(L"releaseKeyboard!!\n");
}

View *UWindow::getKeyboardHolder()
{
	return mFocusHolder;
}


UContextMenu *UWindow::startContextMenu(
	UContextMenuCallback *callback, View *anchor, Gravity gravity)
{
	UContextMenu *contextMenu
		= new UContextMenu(this, callback);

	if (!callback->onCreateContextMenu(
		contextMenu, contextMenu->getMenu()))
	{
		delete contextMenu;
		return nullptr;
	}

	callback->onPrepareContextMenu(
		contextMenu, contextMenu->getMenu());

	if (contextMenu->getMenu()->getItemCount() == 0)
	{
		delete contextMenu;
		return nullptr;
	}

	mContextMenu = contextMenu;

	int x, y;
	URect rect = anchor->getBoundInWindow();

	y = rect.bottom + 1;

	switch (gravity)
	{
	case LEFT:
		x = rect.left;
		break;

	case RIGHT:
		x = rect.right - 92;
		break;

	case CENTER:
		x = rect.left - (92 - rect.width()) / 2.f;
		break;

	default:
		x = rect.left;
	}

	//异步打开TextActionMode菜单，以防止在输入事件处理流程中
	//打开菜单时出现问题。
	class UContextMenuWorker
		: public UExecutable
	{
	private:
		int mX, mY;
		UWindow *window;
	public:
		UContextMenuWorker(UWindow *w, int x, int y)
		{
			mX = x;
			mY = y;
			window = w;
		}
		void run() override
		{
			window->mContextMenu->show(mX, mY);
		}
	}*worker = new UContextMenuWorker(this, x, y);

	mLabourCycler->post(worker);

	return contextMenu;
}

TextActionMode *UWindow::startTextActionMode(
	TextActionModeCallback *callback)
{
	TextActionMode *actionMode
		= new TextActionMode(this, callback);

	if (!callback->onCreateActionMode(
		actionMode, actionMode->getMenu()))
	{
		delete actionMode;
		return nullptr;
	}

	callback->onPrepareActionMode(
		actionMode, actionMode->getMenu());

	if (actionMode->getMenu()->getItemCount() == 0)
	{
		delete actionMode;
		return nullptr;
	}

	mTextActionMode = actionMode;

	//异步打开TextActionMode菜单，以防止在输入事件处理流程中
	//打开菜单时出现问题。
	class TextActionModeWorker
		: public UExecutable
	{
	private:
		UWindow *window;
	public:
		TextActionModeWorker(UWindow *w)
		{
			window = w;
		}
		void run() override
		{
			window->mTextActionMode->show();
		}
	}*worker = new TextActionModeWorker(this);

	mLabourCycler->post(worker);

	return actionMode;
}


bool UWindow::isCreated()
{
	return mIsCreated;
}

bool UWindow::isShowed()
{
	return mIsShowing;
}

bool UWindow::isStartupWindow()
{
	return mApplication->getWindowManager()->getStartupWindow() == mId;
}

bool UWindow::isCursorInClient()
{
	RECT clientRect;
	RECT clientInScreenRect;
	POINT cursorPos;
	POINT convertPos;

	::GetCursorPos(&cursorPos);
	::GetClientRect(mWindowHandle, &clientRect);

	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;

	convertPos.x = clientRect.left;
	convertPos.y = clientRect.top;
	::ClientToScreen(mWindowHandle, &convertPos);

	clientInScreenRect.left = convertPos.x;
	clientInScreenRect.top = convertPos.y;
	clientInScreenRect.right = clientInScreenRect.left + clientWidth;
	clientInScreenRect.bottom = clientInScreenRect.top + clientHeight;

	return (cursorPos.x >= clientInScreenRect.left
		&& cursorPos.x <= clientInScreenRect.right
		&& cursorPos.y >= clientInScreenRect.top
		&& cursorPos.y <= clientInScreenRect.bottom);
}


void UWindow::show()
{
	if (!mIsCreated)
	{
		if (this->onRequestCreate())
		{
			mIsCreated = true;
			mApplication->getWindowManager()->addWindow(this);
			if (this->onRequestShow(true))
				mIsShowing = true;
			else
				throw std::runtime_error("UWindow-show(): request show failed.");
		}
		else
			throw std::runtime_error("UWindow-show(): request create failed.");
	}
	else
	{
		if (mIsShowing)
			return;

		if (this->onRequestShow(true))
			mIsShowing = true;
		else
			throw std::runtime_error("UWindow-show(): request show failed.");
	}

	//mSlave->sync();
}

void UWindow::hide()
{
	if (!mIsCreated || !mIsShowing)
		return;

	if (this->onRequestShow(false))
		mIsShowing = false;
	else
		throw std::runtime_error("UWindow-hide(): request hide failed.");
}

void UWindow::focus()
{
	if (!this->onRequestFocus())
		throw std::runtime_error("UWindow-focus(): request focus failed.");
}

void UWindow::close()
{
	this->close(true);
}

void UWindow::close(bool notify)
{
	if (!mIsCreated)
		throw std::logic_error("UWindow-close(): window is not created.");

	if (notify ? this->onClose() : true)
	{
		mBaseLayout->onDetachedFromWindow();

		if (this->onRequestClose())
		{
			mIsCreated = false;
			mIsShowing = false;
			mApplication->getWindowManager()->removeWindow(this);
		}
		else
			throw std::runtime_error("UWindow-close(): request close failed.");
	}
}


void UWindow::notifyWindowSizeChanged(
	int param,
	int width, int height,
	int clientWidth, int clientHeight)
{
	if (clientWidth <= 0 || clientHeight <= 0)
		return;

	mPrevWidth = mWidth;
	mPrevHeight = mHeight;
	mWidth = width;
	mHeight = height;

	mPrevClientWidth = mClientWidth;
	mPrevClientHeight = mClientHeight;
	mClientWidth = clientWidth;
	mClientHeight = clientHeight;

	this->onResize(param, width, height, clientWidth, clientHeight);

	this->performLayout();
	this->performRefresh();
}

void UWindow::notifyWindowLocationChanged(int x, int y)
{
	mPrevX = mX;
	mX = x;
	mPrevY = mY;
	mY = y;

	RECT slaveRect;
	::GetWindowRect(mSlave->getWindowHandle(), &slaveRect);

	int slaveWidth = slaveRect.right - slaveRect.left;
	int slaveHeight = slaveRect.bottom - slaveRect.top;
	::MoveWindow(mSlave->getWindowHandle(), x - 7, y - 7, slaveWidth, slaveHeight, FALSE);

	this->onMove(x, y);
}


void UWindow::invalidate()
{
	mLabourCycler->removeMessages(SCHEDULE_RENDER);
	mLabourCycler->sendEmptyMessage(SCHEDULE_RENDER);
}

void UWindow::invalidate(int left, int top, int right, int bottom)
{
}

void UWindow::requestLayout()
{
	mLabourCycler->removeMessages(SCHEDULE_LAYOUT);
    mLabourCycler->sendEmptyMessage(SCHEDULE_LAYOUT);
}


void UWindow::performLayout()
{
	if (!mIsCreated)
		return;

	LayoutParams *params = mBaseLayout->getLayoutParams();

	int width = mClientWidth;
	int height = mClientHeight;
	int widthSpec = View::EXACTLY;
	int heightSpec = View::EXACTLY;

	if (params->width < 0)
	{
		switch (params->width)
		{
		case LayoutParams::FIT_CONTENT:
			widthSpec = View::FIT;
			break;

		case LayoutParams::MATCH_PARENT:
			widthSpec = View::EXACTLY;
			break;
		}
	}
	else
	{
		width = params->width;
		widthSpec = View::EXACTLY;
	}

	if (params->height < 0)
	{
		switch (params->height)
		{
		case LayoutParams::FIT_CONTENT:
			heightSpec = View::FIT;
			break;

		case LayoutParams::MATCH_PARENT:
			heightSpec = View::EXACTLY;
			break;
		}
	}
	else
	{
		height = params->height;
		heightSpec = View::EXACTLY;
	}

	mBaseLayout->measure(width, height, widthSpec, heightSpec);

	int measuredWidth = mBaseLayout->getMeasuredWidth();
	int measuredHeight = mBaseLayout->getMeasuredHeight();

	mBaseLayout->layout(0, 0, measuredWidth, measuredHeight);
}

void UWindow::performRefresh()
{
	if (!mIsCreated)
		return;

	URect rect(0, 0, mWidth, mHeight);

	if (!this->onRequestDraw(rect))
		throw std::runtime_error("UWindow-performRefresh(): Render failed.");
}

void UWindow::performRefresh(int left, int top, int right, int bottom)
{
	if (!mIsCreated)
		return;

	URect rect(left, top, right, bottom);

	if (!this->onRequestDraw(rect))
		throw std::runtime_error("UWindow-performRefresh(): Render failed.");
}


bool UWindow::onRequestCreate()
{
	HWND hWnd = ::CreateWindowExW(WS_EX_APPWINDOW,
		getApplication()->getWindowClass()->getWindowClassName().c_str(),
		mTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		mX, mY, mWidth, mHeight, 0, 0,
		::GetModuleHandleW(nullptr), this);

	return (hWnd != nullptr);
}

bool UWindow::onRequestResize(int newWidth, int newHeight, int oldWidth, int oldHeight)
{
	if (mWindowHandle == nullptr)
		return false;

	::MoveWindow(mWindowHandle, mX, mY, newWidth, newHeight, FALSE);

	return true;
}

bool UWindow::onRequestMove(int x, int y, int dx, int dy)
{
	if (mWindowHandle == nullptr)
		return false;

	::MoveWindow(this->mWindowHandle, x, y, mWidth, mHeight, FALSE);

	return true;
}

bool UWindow::onRequestShow(bool show)
{
	if (mWindowHandle == nullptr)
		return false;

	::ShowWindow(this->mWindowHandle, show ? SW_SHOW : SW_HIDE);

	return true;
}

bool UWindow::onRequestFocus()
{
	if (mWindowHandle == nullptr)
		return false;

	::SetFocus(this->mWindowHandle);

	return true;
}

bool UWindow::onRequestDraw(URect rect)
{
	if (mIsCreated)
	{
		getAnimationManager()->update();

		bool ret = mRenderer->render(
			mBackgroundColor,
			[this]() {this->onDraw(mCanvas); });
        if (!ret) {
            exit(0);
        }

		if (getAnimationManager()->isBusy())
			invalidate();
	}

	return true;
}

bool UWindow::onRequestClose()
{
	BOOL succeed = ::DestroyWindow(mWindowHandle);
	return succeed == 0 ? false : true;
}


void UWindow::onCreate()
{
	mSlave = new UWindowSlave(this);
	mLabourCycler = new UpdateCycler(this);

	mBaseLayout = new BaseLayout(this, UWidgetId::ROOT_LAYOUT);
	mBaseLayout->setLayoutParams(
		new LayoutParams(
			LayoutParams::MATCH_PARENT,
			LayoutParams::MATCH_PARENT));

	/*UFrameLayout *titleBar = new UFrameLayout(this);
	titleBar->setBackground(new UColorDrawable(this, UColor::Blue100));
	UBaseLayoutParams *titleBarLp = new UBaseLayoutParams(
		UBaseLayoutParams::MATCH_PARENT, 50);
	mBaseLayout->addContent(titleBar, titleBarLp);*/

	mAnimationManager = new UAnimationManager();
	HRESULT hr = mAnimationManager->init();
	if (FAILED(hr))
		throw std::runtime_error("UWindow-onCreate(): init animation manager failed.");

	mAnimStateChangedListener = new AnimStateChangedListener(this);
	mAnimationManager->setOnStateChangedListener(mAnimStateChangedListener);

	//mAnimationManager->connectTimer(true);
	mAnimTimerEventListener = new AnimTimerEventListener(this);
	//mAnimationManager->setTimerEventListener(mAnimTimerEventListener);

	mRenderer = new URenderer();
	hr = mRenderer->init(this);
	if (FAILED(hr))
		throw std::runtime_error("UWindow-onCreate(): Init DirectX renderer failed.");

	auto deviceContext = mRenderer->getD2DDeviceContext();

	mCanvas = new UCanvas(deviceContext.cast<ID2D1RenderTarget>());
	mBitmapFactory = new UBitmapFactory(this->getApplication()->getWICManager(), deviceContext);

	mBaseLayout->onAttachedToWindow();
}

void UWindow::onShow(bool show)
{
}

void UWindow::onDraw(UCanvas *canvas)
{
	if (canvas && mBaseLayout->isLayouted())
		mBaseLayout->draw(canvas);
}

void UWindow::onActivate(int param)
{
	switch (param)
	{
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		mBaseLayout->dispatchWindowFocusChanged(true);
		/*if (mFocusHolderBackup)
			mFocusHolderBackup->requestFocus();*/
		break;
	case WA_INACTIVE:
		while (mMouseHolderRef > 0)
			this->releaseMouse();
		mBaseLayout->dispatchWindowFocusChanged(false);
		/*if (mFocusHolder)
		{
			UInputEvent ev;
			ev.setEvent(UInputEvent::EVENT_CANCEL);

			mFocusHolderBackup = mFocusHolder;
			mFocusHolderBackup->dispatchInputEvent(&ev);
			mFocusHolder->discardFocus();
		}*/
		break;
	}
}

void UWindow::onMove(int x, int y)
{
}

void UWindow::onResize(
	int param,
	int width, int height,
	int clientWidth, int clientHeight)
{
	switch (param)
	{
	case SIZE_RESTORED:
		break;

	case SIZE_MINIMIZED:
		break;

	case SIZE_MAXIMIZED:
		break;

	case SIZE_MAXSHOW:
		break;

	case SIZE_MAXHIDE:
		break;
	}

	HRESULT hr = mRenderer->resize();
	if (FAILED(hr))
		throw std::runtime_error("UWindow-onResize(): Resize DirectX Renderer failed.");
}

bool UWindow::onMoving(RECT *rect)
{
	return false;
}

bool UWindow::onResizing(WPARAM edge, RECT *rect)
{
	int minWidth = mMinWidth;
	int minHeight = mMinHeight;
	bool processed = false;

	LONG width = rect->right - rect->left;
	LONG height = rect->bottom - rect->top;
	if (height < minHeight)
	{
		switch (edge)
		{
		case WMSZ_TOP:
		case WMSZ_TOPLEFT:
		case WMSZ_TOPRIGHT:
			rect->top = rect->bottom - minHeight;
			break;
		case WMSZ_BOTTOM:
		case WMSZ_BOTTOMLEFT:
		case WMSZ_BOTTOMRIGHT:
			rect->bottom = rect->top + minHeight;
			break;
		}
		processed = true;
	}

	if (width < minWidth)
	{
		switch (edge)
		{
		case WMSZ_LEFT:
		case WMSZ_TOPLEFT:
		case WMSZ_BOTTOMLEFT:
			rect->left = rect->right - minWidth;
			break;
		case WMSZ_RIGHT:
		case WMSZ_TOPRIGHT:
		case WMSZ_BOTTOMRIGHT:
			rect->right = rect->left + minWidth;
			break;
		}
		processed = true;
	}

	return processed;
}

bool UWindow::onClose()
{
	return true;
}

void UWindow::onDestroy()
{
	delete mBaseLayout;

	delete mBitmapFactory;
	mBitmapFactory = nullptr;

	delete mCanvas;
	mCanvas = nullptr;

	mRenderer->close();
	delete mRenderer;
	mRenderer = nullptr;

	delete mAnimTimerEventListener;
	delete mAnimStateChangedListener;

	mAnimationManager->setOnStateChangedListener(0);
	mAnimationManager->close();
	delete mAnimationManager;
	delete mLabourCycler;
	delete mSlave;
}

void UWindow::onInputEvent(UInputEvent *e)
{
	//追踪鼠标，以便产生EVENT_MOUSE_LEAVE_WINDOW事件。
	if (e->getEvent() == UInputEvent::EVENT_MOUSE_LEAVE_WINDOW)
		mIsEnableMouseTrack = true;
	else if (e->getEvent() == UInputEvent::EVENT_MOUSE_MOVE)
		this->setMouseTrack();

	if (e->isMouseEvent())
	{
		//若有之前捕获过鼠标的Widget存在，则直接将所有事件
		//直接发送至该Widget。
		if (mMouseHolder
			&& mMouseHolder->getVisibility() == View::VISIBLE
			&& mMouseHolder->isEnabled())
		{
			//进行坐标变换，将目标Widget左上角映射为(0, 0)。
			int totalLeft = 0;
			int totalTop = 0;
			View *parent = mMouseHolder->getParent();
			while (parent)
			{
				totalLeft += (parent->getLeft() - parent->getScrollX());
				totalTop += (parent->getTop() - parent->getScrollY());

				parent = parent->getParent();
			}

			e->setMouseX(e->getMouseX() - totalLeft);
			e->setMouseY(e->getMouseY() - totalTop);
			e->setIsMouseCaptured(true);

			mMouseHolder->dispatchInputEvent(e);
		}
		else
			mBaseLayout->dispatchInputEvent(e);
	}
	else if (e->isKeyboardEvent())
	{
		if (mFocusHolder)
			mFocusHolder->dispatchInputEvent(e);
	}
}


UWindow::UpdateCycler::UpdateCycler(UWindow *window)
	:UCycler()
{
	this->mWindow = window;
}

void UWindow::UpdateCycler::handleMessage(UMessage *msg)
{
	switch (msg->what)
	{
	case SCHEDULE_RENDER:
		this->mWindow->performRefresh();
		break;
	case SCHEDULE_LAYOUT:
		this->mWindow->performLayout();
		break;
	}
}


UWindow::AnimStateChangedListener::AnimStateChangedListener(UWindow *window)
{
	this->mWindow = window;
}

void UWindow::AnimStateChangedListener::onStateChanged(
	UI_ANIMATION_MANAGER_STATUS newStatus,
	UI_ANIMATION_MANAGER_STATUS previousStatus)
{
	if (newStatus == UI_ANIMATION_MANAGER_BUSY
		&& previousStatus == UI_ANIMATION_MANAGER_IDLE)
		mWindow->invalidate();
}


UWindow::AnimTimerEventListener::AnimTimerEventListener(UWindow *window)
{
	this->mWindow = window;
}

void UWindow::AnimTimerEventListener::OnPreUpdate()
{

}

void UWindow::AnimTimerEventListener::OnPostUpdate()
{
	mWindow->invalidate();
}

void UWindow::AnimTimerEventListener::OnRenderingTooSlow(unsigned int fps)
{
}