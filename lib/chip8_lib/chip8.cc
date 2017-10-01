#include "chip8.h"

#include "controller.h"
#include "cpu.h"
#include "display.h"
#include "random.h"
#include "rom.h"

#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace {

static const size_t CLOCK_SPEED_US = 1852;

} // anonymous namespace

namespace Chip8 {

Chip8::Chip8()
	: mCpu(nullptr),
	  mRom(nullptr),
	  mDisplay(nullptr),
	  mController(nullptr),
	  mRandom(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		throw std::runtime_error("chip8: Failed to initialise SDL.");
	}
}

Chip8::~Chip8()
{
	SDL_Quit();
}

void Chip8::Run(const std::string& path)
{
	try
	{
		mRom.reset(new Rom(path));
		mDisplay.reset(new Display());
		mController.reset(new Controller());
		mRandom.reset(new Random());
		mCpu.reset(new Cpu(*mRom,
		                   *mDisplay,
		                   *mController,
		                   *mRandom));
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		return;
	}

	while (mController->ProcessEvents())
	{
		mCpu->Execute();

		std::this_thread::sleep_for(std::chrono::microseconds(CLOCK_SPEED_US));
	}
}

} // namespace Chip8