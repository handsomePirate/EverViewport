#ifdef __linux__
#include "../WindowAPI.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

inline Display* OpenDisplay()
{
	return XOpenDisplay(0);
}

namespace EverViewport
{
    	struct Window::Private
    	{
        	::XID handle = 0;
        	Display* display = 0;
        	bool shouldClose = false;
        	WindowCallbacks windowCallbacks;
    	};

	Window::Window(int x, int y, int width, int height, const char* title, WindowCallbacks windowCallbacks)
		: p_(new Private)
	{
		p_->display = OpenDisplay();
		auto* display = p_->display;
		auto handle = p_->handle;
		
		if (!display)
		{
			fprintf(stderr, "Cannot open display\n");
		}
		
		int screen = DefaultScreen(display);
		
		handle = XCreateSimpleWindow(display,
			RootWindow(display, screen), x, y, width, height,
			1, BlackPixel(display, screen), WhitePixel(display, screen));
		
		XSelectInput(display, handle, ExposureMask | KeyPressMask);
		
		XMapWindow(display, handle);
	}
	
	Window::~Window()
	{
		XDestroyWindow(p_->display, p_->handle);
		XCloseDisplay(p_->display);
		delete p_;
	}
	
	WindowHandle Window::GetWindowHandle() const
	{
		return p_->handle;
	}
	
	void Window::PollMessages()
	{
		XEvent event;
		XNextEvent(p_->display, &event);
	}
	
	bool Window::ShouldClose() const
	{
		return true;
	}
}

#endif
