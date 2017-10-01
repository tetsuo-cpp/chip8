#pragma once

#include "interfaces.h"

#include <SDL2/SDL.h>

namespace Chip8 {

class Display : public IDisplay
{
public:
	Display();
	virtual ~Display() override;

	void Clear() override;
	bool DrawSprite(uint16_t x,
	                uint16_t y,
	                const std::vector<uint8_t>& sprite) override;
	void Render() const override;

private:
	size_t ToIndex(uint16_t x, uint16_t y) const;
	std::pair<size_t, size_t> FromIndex(size_t index) const;

	SDL_Window*       mWindow;
	SDL_Renderer*     mRenderer;
	std::vector<bool> mPixels;
};

} // namespace Chip8
