#include "../lib/chip8.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return EXIT_FAILURE;
	}

	Chip8::Chip8 app;
	app.Run(argv[1]);

	return EXIT_SUCCESS;
}
