#pragma once

#include "controller.h"
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
	Rom        mRom;
	Display    mDisplay;
	Controller mController;
	Cpu        mCpu;
};

} // namespace Chip8
