#include "chip8.h"

#include <SDL2/SDL.h>

#include <iostream>

namespace Chip8 {

Chip8::Chip8()
	: mCpu(mRom, mDisplay, mController)
{}

void Chip8::Run()
{
	if (!mRom.Init("INVADERS"))
	{
		return;
	}

	SDL_Event e;

	try
	{
		while (true)
		{
			mCpu.EmulateCycle();
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "chip8: Exiting..." << std::endl;
	}
}

} // namespace Chip8
