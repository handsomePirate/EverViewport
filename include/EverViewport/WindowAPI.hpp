#pragma once

#ifdef _WIN32
struct HWND__;
typedef HWND__* HWND;
typedef HWND WindowHandle;

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
typedef HINSTANCE Connection;
#elif __linux__
typedef unsigned long XID;
typedef XID WindowHandle;

typedef void* Connection;
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
		Connection GetProgramConnection() const;

		bool InFocus() const;

		void PollMessages();

		bool ShouldClose() const;
		void Close();

	private:
		struct Private;
		Private* p_;
	};
}
