#include "controller.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

const std::vector<SDL_Scancode> SCANCODES =
{
	SDL_SCANCODE_1,
	SDL_SCANCODE_2,
	SDL_SCANCODE_3,
	SDL_SCANCODE_4,
	SDL_SCANCODE_Q,
	SDL_SCANCODE_W,
	SDL_SCANCODE_E,
	SDL_SCANCODE_R,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_F,
	SDL_SCANCODE_Z,
	SDL_SCANCODE_X,
	SDL_SCANCODE_C,
	SDL_SCANCODE_V
};

} // anonymous namespace

namespace Chip8 {

Controller::Controller()
	: mQuit(false)
{
	assert(SCANCODES.size() == 16);
}

bool Controller::ProcessEvents()
{
	if (mQuit)
	{
		return false;
	}

	while (SDL_PollEvent(&mEvent))
	{
		if (mEvent.type == SDL_QUIT)
		{
			return false;
		}
	}

	return true;
}

uint8_t Controller::WaitForKeyPress()
{
	// If we don't get a keypress we're interested in, just keep waiting.
	while (true)
	{
		SDL_WaitEvent(&mEvent);

		if (mEvent.type == SDL_KEYDOWN)
		{
			for (size_t i = 0; i < SCANCODES.size(); ++i)
			{
				if (mEvent.key.keysym.scancode == SCANCODES[i])
				{
					return i;
				}
			}
		}
		else if (mEvent.type == SDL_QUIT)
		{
			mQuit = true;
			return 0x0;
		}
	}
}

bool Controller::IsKeyPressed(uint8_t key) const
{
	const uint8_t* state = SDL_GetKeyboardState(nullptr);

	return state[SCANCODES[key]];
}

} // namespace Chip8
