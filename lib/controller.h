#pragma once

#include <SDL2/SDL.h>

namespace Chip8 {

class Controller
{
public:
	Controller();

	uint8_t WaitForKey();
	bool IsKeyPressed(uint8_t key);

private:
	SDL_Event mEvent;
};

} // namespace Chip8
