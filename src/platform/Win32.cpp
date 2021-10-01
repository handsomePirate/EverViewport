#ifdef _WIN32

#include "../WindowAPI.hpp"
#include <windows.h>
#include <stdint.h>

#define EW_WINDOW_CLASS_NAME "EverViewport window class"

LRESULT CALLBACK ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

inline HINSTANCE GetHInstance()
{
	return ::GetModuleHandleA(0);
}

inline void DisplayMessageBox(const char* message, const char* title)
{
    ::MessageBoxA(0, message, title, MB_ICONEXCLAMATION | MB_OK);
}

class InternalWindowClass
{
public:
	InternalWindowClass()
	{
        HINSTANCE hInstance = GetHInstance();
        // Setup and register window class.
        HICON icon = LoadIcon(hInstance, IDI_APPLICATION);
        WNDCLASSA wc;
        memset(&wc, 0, sizeof(wc));
        wc.style = CS_DBLCLKS;
        wc.lpfnWndProc = ProcessMessage;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = icon;
        wc.hCursor = NULL;
        wc.hbrBackground = NULL;
        wc.lpszClassName = EW_WINDOW_CLASS_NAME;

        if (!::RegisterClassA(&wc)) {
            DisplayMessageBox("Window class registration failed", "Error");
            return;
        }

        HANDLE consoleHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	~InternalWindowClass()
	{
        ::UnregisterClassA(EW_WINDOW_CLASS_NAME, GetHInstance());
        HANDLE consoleHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
};

static InternalWindowClass windowClass;

namespace EverViewport
{
	Window::Window(int x, int y, int width, int height, const char* title)
	{
        uint32_t windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        uint32_t windowExStyle = WS_EX_APPWINDOW;

        const bool resizable = true;
        if (resizable)
        {
            windowStyle |= WS_MAXIMIZEBOX;
            windowStyle |= WS_MINIMIZEBOX;
            windowStyle |= WS_THICKFRAME;
        }

        RECT borderRectangle = { 0, 0, 0, 0 };
        ::AdjustWindowRectEx(&borderRectangle, windowStyle, 0, windowExStyle);

        uint32_t windowX = x + borderRectangle.left;
        uint32_t windowY = y + borderRectangle.top;
        uint32_t windowWidth = width + borderRectangle.right - borderRectangle.left;
        uint32_t windowHeight = height + borderRectangle.bottom - borderRectangle.top;

        handle_ = (WindowHandle)::CreateWindowExA(
            windowExStyle, EW_WINDOW_CLASS_NAME, title, windowStyle,
            windowX, windowY, windowWidth, windowHeight,
            0, 0, GetHInstance(), 0);

        if (handle_ == 0)
        {
            DisplayMessageBox("Window creation failed", "Error");
            return;
        }

        ::SetPropA((::HWND)handle_, "window ptr", this);

        // TODO: If the window should not accept input, this should be false.
        const bool shouldActivate = true;
        int cmdFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
        // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE.
        // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE.
        ::ShowWindow((::HWND)handle_, cmdFlags);
	}

	Window::~Window()
	{
        ::DestroyWindow((::HWND)handle_);
        handle_ = 0;
	}

	WindowHandle Window::GetWindowHandle() const
	{
		return handle_;
	}
}

LRESULT CALLBACK ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
    }
    else if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP)
    {
        return TRUE;
    }
    else if (msg == WM_CHAR)
    {
        if (wParam > 0 && wParam < 0x10000)
        {
        }
        return TRUE;
    }
    else if (msg == WM_CLOSE)
    {
        return TRUE;
    }
    else if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
    {
        uint16_t whichButton = msg & 0x0001 ? 0 /*left*/ : 1 /*right*/;

        SetCapture(hwnd);

        return TRUE;
    }
    else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP)
    {
        uint16_t whichButton = msg & 0x0001 ? 1 /*left*/ : 0 /*right*/;

        ReleaseCapture();

        return TRUE;
    }
    else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP || msg == WM_MBUTTONDBLCLK)
    {
        return TRUE;
    }
    else if (msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL)
    {
        return TRUE;
    }
    else if (msg == WM_MOUSEMOVE)
    {

    }
    else if (msg == WM_SIZE)
    {
        RECT rectangle;
        ::GetClientRect(hwnd, &rectangle);
    }
    else if (msg == WM_SETCURSOR)
    {

    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

#endif