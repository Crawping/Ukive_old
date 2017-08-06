#include "UCommon.h"
#include "UWindowManager.h"
#include "UApplication.h"
#include "UWindow.h"
#include "UInputEvent.h"
#include "UInputConnection.h"
#include "UWindowClass.h"


UWindowClass::UWindowClass(UApplication *app)
{
	if (app == nullptr)
		throw std::logic_error(
			"UWindowClass-Constructor(): null param.");

	sUWCVtr = this;

	mApplication = app;
	mInputEvent = new UInputEvent();

	if (!createWindowClass(L"UWindowClass"))
		throw std::runtime_error(
			"UWindowClass-Constructor(): create window class failed.");
}


UWindowClass::~UWindowClass()
{
	::UnregisterClassW(
		mWindowClassName.c_str(), 
		::GetModuleHandleW(nullptr));

	delete mInputEvent;
}


bool UWindowClass::createWindowClass(std::wstring className)
{
	if (className.empty())
		mWindowClassName = L"UWindowClass";
	else
		mWindowClassName = className;

	mWindowClassNative.style = CS_HREDRAW | CS_VREDRAW;
	mWindowClassNative.lpfnWndProc = WndProc;
	mWindowClassNative.cbClsExtra = 0;
	mWindowClassNative.cbWndExtra = 0;
	mWindowClassNative.hInstance = ::GetModuleHandleW(nullptr);
	mWindowClassNative.hIcon = LoadIconW(nullptr, IDI_WINLOGO);
	mWindowClassNative.hIconSm = mWindowClassNative.hIcon;
	mWindowClassNative.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	mWindowClassNative.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	mWindowClassNative.lpszMenuName = nullptr;
	mWindowClassNative.lpszClassName = mWindowClassName.c_str();
	mWindowClassNative.cbSize = sizeof(WNDCLASSEXW);

	if (RegisterClassExW(&mWindowClassNative) == 0)
		return false;

	return true;
}


std::wstring UWindowClass::getWindowClassName()
{
	return mWindowClassName;
}


void UWindowClass::processInputEvent(UWindow *window, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UInputEvent *ev = mInputEvent;
	ev->setIsMouseCaptured(false);

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		ev->setEvent(UInputEvent::EVENT_MOUSE_KEY_DOWN);
		ev->setMouseKey(UInputEvent::KEY_MOUSE_LEFT);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_LBUTTONUP:
		ev->setEvent(UInputEvent::EVENT_MOUSE_KEY_UP);
		ev->setMouseKey(UInputEvent::KEY_MOUSE_LEFT);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_RBUTTONDOWN:
		ev->setEvent(UInputEvent::EVENT_MOUSE_KEY_DOWN);
		ev->setMouseKey(UInputEvent::KEY_MOUSE_RIGHT);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_RBUTTONUP:
		ev->setEvent(UInputEvent::EVENT_MOUSE_KEY_UP);
		ev->setMouseKey(UInputEvent::KEY_MOUSE_RIGHT);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_MBUTTONDOWN:
		ev->setEvent(UInputEvent::EVENT_MOUSE_KEY_DOWN);
		ev->setMouseKey(UInputEvent::KEY_MOUSE_MIDDLE);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_MBUTTONUP:
		ev->setEvent(UInputEvent::EVENT_MOUSE_KEY_UP);
		ev->setMouseKey(UInputEvent::KEY_MOUSE_MIDDLE);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEMOVE:
		ev->setEvent(UInputEvent::EVENT_MOUSE_MOVE);
		ev->setMouseX(GET_X_LPARAM(lParam));
		ev->setMouseY(GET_Y_LPARAM(lParam));
		ev->setMouseRawX(GET_X_LPARAM(lParam));
		ev->setMouseRawY(GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSELEAVE:
		ev->setEvent(UInputEvent::EVENT_MOUSE_LEAVE_WINDOW);
		break;

	case WM_MOUSEHOVER:
		ev->setEvent(UInputEvent::EVENT_MOUSE_HOVER);
		break;

	case WM_MOUSEWHEEL:
	{
		::POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(window->getWindowHandle(), &pt);

		ev->setEvent(UInputEvent::EVENT_MOUSE_WHEEL);
		ev->setMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		ev->setMouseX(pt.x);
		ev->setMouseY(pt.y);
		ev->setMouseRawX(pt.x);
		ev->setMouseRawY(pt.y);
		break;
	}

	case WM_KEYDOWN:
		ev->setEvent(UInputEvent::EVENT_KEYBOARD_KEY_DOWN);
		ev->setKeyboardKey(wParam, lParam);
		break;

	case WM_KEYUP:
		ev->setEvent(UInputEvent::EVENT_KEYBOARD_KEY_UP);
		ev->setKeyboardKey(wParam, lParam);
		break;
	}

	window->onInputEvent(ev);
}


LRESULT CALLBACK UWindowClass::messageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCTW *cs = (CREATESTRUCTW*)lParam;
		UWindow *window = (UWindow*)cs->lpCreateParams;
		if (window == nullptr)
			throw std::runtime_error("UWindowClass::messageHandler(): null create param.");

		window->setHandleForCreate(hWnd);
		::SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG)window);

		return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	UWindow *window = (UWindow*)::GetWindowLongPtrW(hWnd, GWL_USERDATA);
	if (window != nullptr)
	{
		switch (uMsg)
		{
		case WM_CREATE:
			window->onCreate();
			return TRUE;

		case WM_CLOSE:
			if (mApplication->getAppQuitStrategy() == UApplication::QUIT_WHEN_STARTUP_WINDOW_CLOSED
				&& window->isStartupWindow())
			{
				if (window->onClose())
				{
					for (std::size_t i = 0; i < mApplication->getWindowManager()->getWindowCount(); ++i)
					{
						UWindow *pcWindow = mApplication->getWindowManager()->getWindowByPos(i);
						if (!pcWindow->isStartupWindow())
							pcWindow->close();
					}
					window->close(false);
				}
			}
			else
				window->close();
			return 0;

		case WM_DESTROY:
			if (mApplication->getAppQuitStrategy() == UApplication::QUIT_WHEN_STARTUP_WINDOW_CLOSED)
			{
				window->onDestroy();
				if (window->isStartupWindow())
					::PostQuitMessage(0);
			}
			else if (mApplication->getAppQuitStrategy() == UApplication::QUIT_WHEN_LAST_WINDOW_CLOSED)
			{
				window->onDestroy();
				if (mApplication->getWindowManager()->getWindowCount() == 1)
					::PostQuitMessage(0);
			}
			return 0;

		case WM_SHOWWINDOW:
			window->onShow((BOOL)wParam == TRUE ? true : false);
			break;

		case WM_ACTIVATE:
		{
			window->onActivate(LOWORD(wParam));
			break;
		}

		case WM_ERASEBKGND:
			return TRUE;

		case WM_SETCURSOR:
		{
			if (window->isCursorInClient())
			{
				::SetCursor(window->getCurrentCursor());
				return TRUE;
			}
			break;
		}

		case WM_MOVE:
			window->notifyWindowLocationChanged(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_SIZE:
		{
			RECT winRect;
			::GetWindowRect(hWnd, &winRect);

			window->notifyWindowSizeChanged(
				wParam,
				winRect.right - winRect.left, winRect.bottom - winRect.top,
				LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		case WM_MOVING:
		{
			if (window->onMoving((RECT*)lParam))
				return TRUE;
			break;
		}

		case WM_SIZING:
		{
			if (window->onResizing(wParam, (RECT*)lParam))
				return TRUE;
			break;
		}

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSELEAVE:
		case WM_MOUSEHOVER:
		case WM_MOUSEWHEEL:
		case WM_KEYDOWN:
		case WM_KEYUP:
			this->processInputEvent(window, uMsg, wParam, lParam);
			return 0;
		}
	}

	return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


LRESULT UWindowClass::processDWMProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool* pfCallDWP)
{
	LRESULT lRet = 0;
	HRESULT hr = S_OK;
	bool fCallDWP = true; // Pass on to DefWindowProc?

	fCallDWP = !DwmDefWindowProc(hWnd, message, wParam, lParam, &lRet);

	// Handle window creation.
	if (message == WM_CREATE)
	{
		RECT rcClient;
		GetWindowRect(hWnd, &rcClient);

		// Inform application of the frame change.
		SetWindowPos(hWnd,
			0,
			rcClient.left, rcClient.top,
			rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
			SWP_FRAMECHANGED);

		fCallDWP = true;
		lRet = 0;
	}

	// Handle window activation.
	if (message == WM_ACTIVATE)
	{
		// Extend the frame into the client area.
		MARGINS margins;

		margins.cxLeftWidth = 8;
		margins.cxRightWidth = 8;
		margins.cyBottomHeight = 8;
		margins.cyTopHeight = 8;

		hr = DwmExtendFrameIntoClientArea(hWnd, &margins);

		if (!SUCCEEDED(hr))
		{
			// Handle error.
		}

		fCallDWP = true;
		lRet = 0;
	}

	if (message == WM_PAINT)
	{
		fCallDWP = true;
		lRet = 0;
	}

	// Handle the non-client size message.
	if (message == WM_NCCALCSIZE && wParam == TRUE)
	{
		// Calculate new NCCALCSIZE_PARAMS based on custom NCA inset.
		NCCALCSIZE_PARAMS *pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

		RECT newW, oldW, oldWC;
		CopyRect(&newW, &pncsp->rgrc[0]);
		//new window client.
		pncsp->rgrc[0].left = newW.left + 0;
		pncsp->rgrc[0].top = newW.top + 0;
		pncsp->rgrc[0].right = newW.right - 0;
		pncsp->rgrc[0].bottom = newW.bottom - 0;

		lRet = 0;

		// No need to pass the message on to the DefWindowProc.
		fCallDWP = false;
	}

	// Handle hit testing in the NCA if not handled by DwmDefWindowProc.
	if ((message == WM_NCHITTEST) && (lRet == 0))
	{
		lRet = HitTestNCA(hWnd, wParam, lParam, 8, 8, 8, 8);

		if (lRet != HTNOWHERE)
		{
			fCallDWP = false;
		}
	}

	*pfCallDWP = fCallDWP;

	return lRet;
}

LRESULT UWindowClass::HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam, 
	int leftExt, int topExt, int rightExt, int bottomExt)
{
	// Get the point coordinates for the hit test.
	POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	// Get the window rectangle.
	RECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);

	// Get the frame rectangle, adjusted for the style without a caption.
	//RECT rcFrame = { 0 };
	//AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

	// Determine if the hit test is for resizing. Default middle (1,1).
	USHORT uRow = 1;
	USHORT uCol = 1;
	//bool fOnResizeBorder = false;

	// Determine if the point is at the top or bottom of the window.
	if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + topExt)
	{
		//fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
		uRow = 0;
	}
	else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - bottomExt)
	{
		uRow = 2;
	}

	// Determine if the point is at the left or right of the window.
	if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + leftExt)
	{
		uCol = 0; // left side
	}
	else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - rightExt)
	{
		uCol = 2; // right side
	}

	// Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
	LRESULT hitTests[3][3] =
	{
		{ HTTOPLEFT,    HTCAPTION,    HTTOPRIGHT },
		{ HTLEFT,       HTNOWHERE,     HTRIGHT },
		{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
	};

	return hitTests[uRow][uCol];
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool callDWP = true;
	BOOL dwmEnabled = FALSE;
	LRESULT lRet = 0;
	HRESULT hr = S_OK;

	// Winproc worker for custom frame issues.
	/*hr = DwmIsCompositionEnabled(&dwmEnabled);
	if (SUCCEEDED(hr))
	{
		lRet = sUWCVtr->processDWMProc(hWnd, uMsg, wParam, lParam, &callDWP);
	}*/

	// Winproc worker for the rest of the application.
	if (callDWP)
	{
		lRet = sUWCVtr->messageHandler(hWnd, uMsg, wParam, lParam);
	}

	return lRet;
}