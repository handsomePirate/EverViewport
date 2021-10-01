#pragma once

namespace EverViewport
{

#ifdef _WIN32
	class HWND__;
	typedef HWND__* HWND;
	typedef HWND WindowHandle;
#endif

	class Window
	{
	public:
		Window(int x, int y, int width, int height, const char* title);
		~Window();

		WindowHandle GetWindowHandle() const;

	private:
		// Currently, we don't do pimpl so that another heap allocation is not necessary.
		WindowHandle handle_;
	};
}