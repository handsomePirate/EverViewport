#include <EverViewport/WindowAPI.hpp>
#include <SoftwareCore/EventSystem.hpp>
#include <iostream>

EverViewport::Window* window1 = nullptr;
EverViewport::Window* window2 = nullptr;

void Render()
{
	std::cout << "render" << std::endl;
}

void Resize(int width, int height)
{
	std::cout << "resize: " << width << ", " << height << std::endl;
}

void MouseClick(Core::EventCode eventCode, Core::EventData eventData)
{
	if (window1 && window1->InFocus())
	{
		std::cout << "mouse click" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	volatile int listener = 0;
	CoreEventSystem.SubscribeToEvent(Core::EventCode::MouseButtonPressed, MouseClick, (void*)&listener);

	EverViewport::WindowCallbacks windowCallbacks{ Render, Resize };

	window1 = new EverViewport::Window(50, 50, 720, 480, "test window 1", windowCallbacks);
	window2 = new EverViewport::Window(60, 60, 720, 480, "test window 2", windowCallbacks);
	
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

	CoreEventSystem.UnsubscribeFromEvent(Core::EventCode::MouseButtonPressed, (void*)&listener);

	return 0;
}