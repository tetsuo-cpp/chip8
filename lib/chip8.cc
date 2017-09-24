#include "chip8.h"

#include <SDL2/SDL.h>

#include <iostream>

namespace Chip8 {

Chip8::Chip8()
	: mCpu(mRom, mDisplay)
{}

void Chip8::Run()
{
	if (!mRom.Init("INVADERS"))
	{
		return;
	}

	SDL_Event e;

	while (true)
	{
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				std::cout << "chip8: Detected quit." << std::endl;
				return;
			}
		}

		mCpu.EmulateCycle();
	}
}

} // namespace Chip8
