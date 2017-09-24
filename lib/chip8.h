#pragma once

#include "cpu.h"
#include "display.h"
#include "rom.h"

namespace Chip8 {

class Chip8
{
public:
	Chip8();

	void Run();

private:
	Rom     mRom;
	Cpu     mCpu;
	Display mDisplay;
};

} // namespace Chip8
