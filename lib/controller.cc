#include "controller.h"

#include <cassert>
#include <stdexcept>
#include <vector>

namespace {

static const std::vector<SDL_Scancode> SCANCODES =
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
{
	assert(SCANCODES.size() == 16);
}

uint8_t Controller::WaitForKey()
{
	// If we don't get a keypress we're interested in, just keep waiting.
	while (true)
	{
		SDL_WaitEvent(&mEvent);

		switch (mEvent.type)
		{
		case SDL_KEYDOWN:
		{
			for (size_t i = 0; i < SCANCODES.size(); ++i)
			{
				if (mEvent.key.keysym.scancode == SCANCODES[i])
				{
					return i;
				}
			}

			break;
		}
		case SDL_QUIT:
		{
			throw std::runtime_error("Captured exit event.");
		}
		default:
			break;
		}
	}
}

bool Controller::IsKeyPressed(uint8_t key)
{
	// Process all events that happened since the last keypress.
	while (SDL_PollEvent(&mEvent))
	{
		if (mEvent.type == SDL_QUIT)
		{
			throw std::runtime_error("Captured exit event.");
		}
	}

	const uint8_t* state = SDL_GetKeyboardState(nullptr);

	return state[SCANCODES[key]];
}

} // namespace Chip8
