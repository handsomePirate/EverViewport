#include <EverViewport/WindowAPI.hpp>
#include <iostream>

void Render()
{
	std::cout << "render" << std::endl;
}

void Resize(int width, int height)
{
	std::cout << "resize: " << width << ", " << height << std::endl;
}

int main(int argc, char* argv[])
{
	EverViewport::WindowCallbacks windowCallbacks{ Render, Resize };

	EverViewport::Window* window1 = new EverViewport::Window(50, 50, 720, 480, "test window 1", windowCallbacks);
	EverViewport::Window* window2 = new EverViewport::Window(60, 60, 720, 480, "test window 2", windowCallbacks);
	
	while ((window1 && !window1->ShouldClose()) || (window2 && !window2->ShouldClose()))
	{
		if (window1 && window1->ShouldClose())
		{
			delete window1;
			window1 = nullptr;
		}

		if (window2 && window2->ShouldClose())
		{
			delete window2;
			window2 = nullptr;
		}

		if (window1)
		{
			window1->PollMessages();
		}

		if (window2)
		{
			window2->PollMessages();
		}
	}

	return 0;
}