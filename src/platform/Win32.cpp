#ifdef _WIN32

#include "EverViewport/WindowAPI.hpp"
#include <SoftwareCore/EventSystem.hpp>
#include <windows.h>
#include <stdint.h>
#include <memory>

#include <iostream>

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
    struct WindowPrivate
    {
        ::HWND handle = 0;
        bool shouldClose = false;
        WindowCallbacks windowCallbacks;
        int width;
        int height;
    };

    struct Window::Private
    {
        unsigned char data[sizeof(WindowPrivate)];
    };

	Window::Window(int x, int y, int width, int height, const char* title, WindowCallbacks windowCallbacks)
        : p_((Private*)(new((void*)new Private) WindowPrivate()))
	{
        ((WindowPrivate*)p_)->windowCallbacks = windowCallbacks;
        ((WindowPrivate*)p_)->width = width;
        ((WindowPrivate*)p_)->height = height;

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

        ((WindowPrivate*)p_)->handle = ::CreateWindowExA(
            windowExStyle, EW_WINDOW_CLASS_NAME, title, windowStyle,
            windowX, windowY, windowWidth, windowHeight,
            0, 0, GetHInstance(), 0);

        if (((WindowPrivate*)p_)->handle == 0)
        {
            DisplayMessageBox("Window creation failed", "Error");
            return;
        }

        ::SetPropA(((WindowPrivate*)p_)->handle, "window ptr", p_);

        // TODO: If the window should not accept input, this should be false.
        const bool shouldActivate = true;
        int cmdFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
        // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE.
        // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE.
        ::ShowWindow(((WindowPrivate*)p_)->handle, cmdFlags);
	}

	Window::~Window()
	{
        ::DestroyWindow(((WindowPrivate*)p_)->handle);
        delete p_;
	}

    WindowHandle Window::GetWindowHandle() const
    {
        return (WindowHandle)((WindowPrivate*)p_)->handle;
    }

    Connection Window::GetProgramConnection() const
    {
        return GetHInstance();
    }

    void Window::PollMessages()
    {
        MSG message;
        while (::PeekMessageA(&message, ((WindowPrivate*)p_)->handle, 0, 0, PM_REMOVE) > 0)
        {
            ::TranslateMessage(&message);
            ::DispatchMessageA(&message);
        }
    }

    bool Window::ShouldClose() const
    {
        return ((WindowPrivate*)p_)->shouldClose;
    }
}

LRESULT CALLBACK ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_PAINT)
    {
        auto handle = (EverViewport::WindowPrivate*)::GetPropA(hwnd, "window ptr");
        
        if (handle->windowCallbacks.renderFunction)
        {
            handle->windowCallbacks.renderFunction();
        }
    }
    else if (msg == WM_CREATE)
    {
        ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
    }
    else if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP)
    {
        Core::EventCode code = (msg & 0x0001) ? Core::EventCode::KeyReleased : Core::EventCode::KeyPressed;
        Core::EventData eventData;
        eventData.data.u16[0] = (uint16_t)wParam;

        CoreEventSystem.SignalEvent(code, eventData);
        return TRUE;
    }
    else if (msg == WM_CHAR)
    {
        if (wParam > 0 && wParam < 0x10000)
        {
            Core::EventData eventData;
            eventData.data.u16[0] = (unsigned short)wParam;
            CoreEventSystem.SignalEvent(Core::EventCode::CharacterInput, eventData);
        }
        return TRUE;
    }
    else if (msg == WM_CLOSE)
    {
        auto handle = (EverViewport::WindowPrivate*)::GetPropA(hwnd, "window ptr");
        handle->shouldClose = true;
        
        return TRUE;
    }
    else if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
    {
        uint16_t whichButton = msg & 0x0001 ? 0 /*left*/ : 1 /*right*/;
        Core::EventData eventData;
        eventData.data.u16[0] = whichButton;

        SetCapture(hwnd);

        CoreEventSystem.SignalEvent(Core::EventCode::MouseButtonPressed, eventData);
        return TRUE;
    }
    else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP)
    {
        uint16_t whichButton = msg & 0x0001 ? 1 /*left*/ : 0 /*right*/;
        Core::EventData eventData;
        eventData.data.u16[0] = whichButton;

        ReleaseCapture();

        CoreEventSystem.SignalEvent(Core::EventCode::MouseButtonReleased, eventData);
        return TRUE;
    }
    else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP || msg == WM_MBUTTONDBLCLK)
    {
        Core::EventCode code = msg & 0x0001 ? Core::EventCode::MouseButtonPressed : Core::EventCode::MouseButtonReleased;
        Core::EventData eventData;
        eventData.data.u16[0] = uint16_t(2);

        if (code == Core::EventCode::MouseButtonPressed)
        {
            SetCapture(hwnd);
        }
        else
        {
            ReleaseCapture();
        }

        CoreEventSystem.SignalEvent(code, eventData);
        return TRUE;
    }
    else if (msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL)
    {
        Core::EventData eventData;
        eventData.data.i8[0] = (int8_t)(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
        eventData.data.u8[1] = (msg == WM_MOUSEHWHEEL) ? 1 : 0;

        CoreEventSystem.SignalEvent(Core::EventCode::MouseWheel, eventData);
        return TRUE;
    }
    else if (msg == WM_MOUSEMOVE)
    {
        POINT pos;
        ::GetCursorPos(&pos);
        Core::EventData eventData;
        eventData.data.i16[0] = (int16_t)pos.x;
        eventData.data.i16[1] = (int16_t)pos.y;
        CoreEventSystem.SignalEvent(Core::EventCode::MouseMoved, eventData);
    }
    else if (msg == WM_SIZE)
    {
        auto width = LOWORD(lParam);
        auto height = HIWORD(lParam);

        auto handle = (EverViewport::WindowPrivate*)::GetPropA(hwnd, "window ptr");
        if (handle->windowCallbacks.resizeFunction)
        {
            handle->windowCallbacks.resizeFunction(width, height);
        }

        if (handle->width > width || handle->height > height)
        {
            if (handle->windowCallbacks.renderFunction)
            {
                handle->windowCallbacks.renderFunction();
            }
        }

        handle->width = width;
        handle->height = height;
    }
    else if (msg == WM_SETCURSOR)
    {
        auto loword = LOWORD(lParam);
        if (loword == HTRIGHT || loword == HTLEFT)
        {
            ::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
        }
        else if (loword == HTTOP || loword == HTBOTTOM)
        {
            ::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
        }
        else if (loword == HTBOTTOMRIGHT || loword == HTTOPLEFT)
        {
            ::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
        }
        else if (loword == HTBOTTOMLEFT || loword == HTTOPRIGHT)
        {
            ::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
        }
        else
        {
            ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
        }
        return TRUE;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

#endif