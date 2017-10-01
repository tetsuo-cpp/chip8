#pragma once

#include "interfaces.h"

#include <SDL2/SDL.h>

namespace Chip8 {

class Controller : public IController
{
public:
	Controller();

	bool ProcessEvents() override;
	uint8_t WaitForKeyPress() override;
	bool IsKeyPressed(uint8_t key) const override;

private:
	SDL_Event mEvent;
	bool mQuit;
};

} // namespace Chip8
