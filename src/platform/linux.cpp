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
		
		if (!p_->display)
		{
			fprintf(stderr, "Cannot open display\n");
		}
		
		int screen = DefaultScreen(p_->display);
		
		p_->handle = XCreateSimpleWindow(p_->display,
			RootWindow(p_->display, screen), x, y, width, height,
			1, BlackPixel(p_->display, screen), WhitePixel(p_->display, screen));
		
		XSelectInput(p_->display, handle, ExposureMask | KeyPressMask);
		
		XMapWindow(p_->display, handle);
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