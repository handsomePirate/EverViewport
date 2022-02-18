#ifdef __linux__

#include "EverViewport/WindowAPI.hpp"
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
        	bool shouldClose = true;
        	WindowCallbacks windowCallbacks;
    	};

	Window::Window(int x, int y, int width, int height, const char* title, WindowCallbacks windowCallbacks)
		: p_(new Private)
	{
		p_->display = OpenDisplay();
		
		if (!p_->display)
		{
			fprintf(stderr, "Cannot open display\n");
			return;
		}
		
		int screen = DefaultScreen(p_->display);
		
		p_->handle = XCreateSimpleWindow(p_->display,
			RootWindow(p_->display, screen), x, y, width, height,
			1, BlackPixel(p_->display, screen), WhitePixel(p_->display, screen));
		
		XSelectInput(p_->display, p_->handle, ExposureMask | KeyPressMask);
		
		XMapWindow(p_->display, p_->handle);
		p_->shouldClose = false;
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

	Connection Window::GetProgramConnection() const
	{
		return (Connection)p_->display;
	}

	bool Window::InFocus() const
	{
		::Window focused;
		int revert_to;

		XGetInputFocus(p_->display, &focused, &revert_to);
		return focused == p_->handle;
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
