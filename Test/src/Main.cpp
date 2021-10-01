#include <WindowAPI.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
	EverViewport::Window window(50, 50, 720, 480, "test window");
	auto handle = window.GetWindowHandle();
	std::cout << handle << std::endl;
	std::cin.get();

	return 0;
}