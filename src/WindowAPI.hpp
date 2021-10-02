#pragma once

namespace EverViewport
{

#ifdef _WIN32
	class HWND__;
	typedef HWND__* HWND;
	typedef HWND WindowHandle;
#endif

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