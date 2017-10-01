#include "display.h"

#include <iostream>
#include <tuple>

namespace {

static const size_t DISPLAY_WIDTH  = 64;
static const size_t DISPLAY_HEIGHT = 32;
static const size_t SPRITE_WIDTH   = 8;
static const size_t PIXEL_SIZE     = 10;

} // namespace anonymous

namespace Chip8 {

Display::Display()
	: mWindow(nullptr),
	  mRenderer(nullptr),
	  mPixels((DISPLAY_WIDTH * DISPLAY_HEIGHT), false)
{
	mWindow = SDL_CreateWindow("Chip8 Emulator",
	                           SDL_WINDOWPOS_CENTERED,
	                           SDL_WINDOWPOS_CENTERED,
	                           DISPLAY_WIDTH * PIXEL_SIZE,
	                           DISPLAY_HEIGHT * PIXEL_SIZE,
	                           SDL_WINDOW_SHOWN);
	if (!mWindow)
	{
		throw std::runtime_error("display: Failed to create SDL window.");
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
	if (!mRenderer)
	{
		SDL_DestroyWindow(mWindow);
		throw std::runtime_error("display: Failed to create SDL renderer.");
	}
}

Display::~Display()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Display::Clear()
{
	std::fill(mPixels.begin(),
	          mPixels.end(),
	          false);
}

bool Display::DrawSprite(uint16_t x,
                         uint16_t y,
                         const std::vector<uint8_t>& sprite)
{
	bool unset = false;
	for (size_t i = 0; i < sprite.size(); ++i)
	{
		uint8_t row = sprite[i];
		for (size_t j = 0; j < SPRITE_WIDTH; ++j)
		{
			bool currentBit = row & 0x80 >> j;
			if (currentBit)
			{
				// This means that we are unsetting a previously set pixel.
				if (mPixels[ToIndex(x + j, y + i)])
				{
					unset = true;
				}

				mPixels[ToIndex(x + j, y + i)] = !(mPixels[ToIndex(x + j, y + i)]);
			}
		}
	}

	return unset;
}

void Display::Render() const
{
	// Black background.
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	SDL_Rect r;
	size_t   x;
	size_t   y;

	// Draw pixels in white.
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	for (size_t i = 0; i < mPixels.size(); ++i)
	{
		if (mPixels[i])
		{
			std::tie(x, y) = FromIndex(i);

			r.x = x * PIXEL_SIZE;
			r.y = y * PIXEL_SIZE;
			r.w = r.h = PIXEL_SIZE;

			SDL_RenderFillRect(mRenderer, &r);
		}
	}

	SDL_RenderPresent(mRenderer);
}

size_t Display::ToIndex(uint16_t x, uint16_t y) const
{
	return x + (y * DISPLAY_WIDTH);
}

std::pair<size_t, size_t> Display::FromIndex(size_t index) const
{
	size_t x = index % DISPLAY_WIDTH;
	size_t y = index / DISPLAY_WIDTH;

	return std::make_pair(x, y);
}

} // namespace Chip8
