#pragma once

#ifdef _WIN32
struct HWND__;
typedef HWND__* HWND;
typedef HWND WindowHandle;
#elif __linux__
typedef unsigned long XID;
typedef XID WindowHandle;
#endif

namespace EverViewport
{
	struct WindowCallbacks
	{
		typedef void(*RenderFnc)();
		RenderFnc renderFunction;
		typedef void(*ResizeFnc)(int, int);
		ResizeFnc resizeFunction;
	};

	class Window
	{
	public:
		Window(int x, int y, int width, int height, const char* title, WindowCallbacks windowCallbacks);
		~Window();

		WindowHandle GetWindowHandle() const;
		void PollMessages();
		bool ShouldClose() const;

	private:
		struct Private;
		Private* p_;
	};
}
