#include <chip8_lib/chip8.h>

#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "./chip8 [filename]" << std::endl;
		return EXIT_FAILURE;
	}

	Chip8::Chip8 app;
	if (!app.Run(argv[1]))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
