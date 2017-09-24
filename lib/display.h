#pragma once

#include <SDL2/SDL.h>

#include <vector>

namespace Chip8 {

class Display
{
public:
	Display();
	virtual ~Display();

	void Clear();
	bool DrawSprite(uint16_t x,
	                uint16_t y,
	                const std::vector<uint8_t>& sprite);
	void Render() const;

private:
	size_t ToIndex(uint16_t x, uint16_t y) const;
	std::pair<size_t, size_t> FromIndex(size_t index) const;

	SDL_Window*       mWindow;
	SDL_Renderer*     mRenderer;
	std::vector<bool> mPixels;
};

} // namespace Chip8
